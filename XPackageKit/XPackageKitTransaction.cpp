#include "XPackageKitTransaction.hpp"
#include "PackageKitBackend.hpp"

#include <QDebug>

XPackageKitTransaction::XPackageKitTransaction(RequestType type, QObject *parent) :
    XTransaction(type, parent)
{
}

QString XPackageKitTransaction::packageName() const
{
    return requestDetails().value("packageName").toString();
}

QString XPackageKitTransaction::repoName() const
{
    return requestDetails().value("repoName").toString();
}

void XPackageKitTransaction::search()
{
    qDebug() << Q_FUNC_INFO << packageName();
    PackageKit::Transaction *rpc = PackageKitBackend::mkResolveTransaction(packageName(),
                                                                           PackageKit::Transaction::FilterBasename
                                                                           | PackageKit::Transaction::FilterNewest);
    connect(rpc, &PackageKit::Transaction::package, this, &XPackageKitTransaction::onSearchResult);
    connect(rpc, &PackageKit::Transaction::finished, this, &XPackageKitTransaction::onSearchTransactionFinished);
    connect(rpc, &PackageKit::Transaction::errorCode, this, &XPackageKitTransaction::onTransactionErrorCode);
#ifdef NEMO_PACKAGE_KIT
    connect(rpc, &PackageKit::Transaction::changed, this, &XPackageKitTransaction::onStatusChanged);
#else
    connect(rpc, &PackageKit::Transaction::statusChanged, this, &XPackageKitTransaction::onStatusChanged);
#endif
}

void XPackageKitTransaction::refresh(const QString &repoName)
{
    qDebug() << Q_FUNC_INFO << repoName;
    PackageKit::Transaction *rpc = PackageKitBackend::mkRefreshRepoTransaction(repoName);
    connect(rpc, &PackageKit::Transaction::finished, this, &XPackageKitTransaction::onGenericTransactionFinished);
    connect(rpc, &PackageKit::Transaction::errorCode, this, &XPackageKitTransaction::onTransactionErrorCode);
#ifdef NEMO_PACKAGE_KIT
    connect(rpc, &PackageKit::Transaction::changed, this, &XPackageKitTransaction::onStatusChanged);
#else
    connect(rpc, &PackageKit::Transaction::statusChanged, this, &XPackageKitTransaction::onStatusChanged);
#endif
}

void XPackageKitTransaction::setRepoEnabled(const QString &repoName, bool enable)
{
    qDebug() << Q_FUNC_INFO << repoName << enable;
    PackageKit::Transaction *rpc = PackageKitBackend::mkSetRepoEnabledTransaction(repoName, enable);
    connect(rpc, &PackageKit::Transaction::finished, this, &XPackageKitTransaction::onGenericTransactionFinished);
    connect(rpc, &PackageKit::Transaction::errorCode, this, &XPackageKitTransaction::onTransactionErrorCode);
#ifdef NEMO_PACKAGE_KIT
    connect(rpc, &PackageKit::Transaction::changed, this, &XPackageKitTransaction::onStatusChanged);
#else
    connect(rpc, &PackageKit::Transaction::statusChanged, this, &XPackageKitTransaction::onStatusChanged);
#endif
}

XPackageKitTransaction::PackageArgs XPackageKitTransaction::getExactSearchResult() const
{
    for (const PackageArgs args : m_searchResult) {
        if (PackageKit::Transaction::packageName(args.packageID) == packageName()) {
            return args;
        }
    }
    return PackageArgs();
}

void XPackageKitTransaction::startEvent()
{
    switch(type()) {
    case RefreshRepoRequest:
        refresh(repoName());
        break;
    case SetRepoEnabledRequest:
    {
        const bool enable = m_requestDetails.value(QStringLiteral("enable"), true).toBool();
        setRepoEnabled(repoName(), enable);
    }
        break;
    case InstallRequest:
    case RemoveRequest:
    case UpdateRequest:
    case StatusRequest:
        search(); // For all those requests we need to search the package first
        break;
    default:
        qWarning() << "Invalid transaction";
        setDelayedFinishedWithError(QVariantMap({{"text", "Invalid transaction type"}}));
        break;
    }
}

void XPackageKitTransaction::onSearchResult(PackageKit::Transaction::Info info, const QString &packageID, const QString &summary)
{
    qDebug() << "Search result (one of):" << packageID << info;
    const PackageArgs args { info, packageID, summary };
    m_searchResult << args;
}

void XPackageKitTransaction::onSearchTransactionFinished(PackageKit::Transaction::Exit status, uint runtime)
{
    qDebug() << Q_FUNC_INFO << packageName() << "result:" << status << runtime;
    PackageArgs result = getExactSearchResult();

    if (result.packageID.isEmpty()) {
        setFinishedWithError({
                                 {"backend_error", PackageKit::Transaction::ErrorPackageNotFound},
                                 {"backend_details", tr("Package not found")}
                             });
        return;
    }

    qDebug() << "Has results {" << result.info << ", " << result.packageID << "); request type: " << m_type;

    switch (m_type) {
    case InstallRequest:
        if (result.info == PackageKit::Transaction::InfoInstalled) {
            qDebug() << Q_FUNC_INFO << "NOP";
            setFinished();
            // NOP
        } else {
            doInstall(result.packageID);
        }
        break;
    case RemoveRequest:
        if (result.info == PackageKit::Transaction::InfoInstalled) {
            doRemove(result.packageID);
        } else {
            qDebug() << Q_FUNC_INFO << "NOP";
            setFinished();
        }
        break;
    case UpdateRequest:
        if (result.info == PackageKit::Transaction::InfoInstalled) {
            doUpdate(result.packageID);
        } else {
            doInstall(result.packageID);
        }
        break;
    case StatusRequest:
        qDebug() << Q_FUNC_INFO << "NOP";
        setFinished();
        break;
    default:
        break;
    }
}

void XPackageKitTransaction::onTransactionErrorCode(PackageKit::Transaction::Error error, const QString &details)
{
    m_errors.append({ error, details });
}

void XPackageKitTransaction::onGenericTransactionFinished(PackageKit::Transaction::Exit exitStatus, uint runtime)
{
    PackageKit::Transaction *t = qobject_cast<PackageKit::Transaction*>(sender());
    qDebug() << Q_FUNC_INFO << "result:" << exitStatus << "runtime:" << runtime << "status" << t->status();

    QVariantMap details = {
        {"backend_exitCode", exitStatus},
        {"backend_runTime", runtime}
    };

    if (!m_errors.isEmpty()) {
        details["backend_errorCode"] = static_cast<int>(m_errors.last().error);
        details["backend_errorDetails"] = m_errors.last().details;
    }

    switch (exitStatus) {
    case PackageKit::Transaction::ExitSuccess:
        setFinished(); // Installed
        break;
    default:
        setFinishedWithError(details);
        break;
    }
}

void XPackageKitTransaction::onStatusChanged()
{
    PackageKit::Transaction *t = qobject_cast<PackageKit::Transaction*>(sender());
    qDebug() << "Status:" << t->status();
}

void XPackageKitTransaction::doInstall(const QString &packageId)
{
    PackageKit::Transaction *rpc = PackageKitBackend::mkInstallPackageTransaction(packageId, PackageKit::Transaction::TransactionFlagNone);
    QObject::connect(rpc, &PackageKit::Transaction::errorCode, [this](PackageKit::Transaction::Error error, const QString &details) {
        setFinishedWithError({{"backend_error", error}, {"backend_details", details}});
        qDebug() << error << details;
    });
    connect(rpc, &PackageKit::Transaction::errorCode, this, &XPackageKitTransaction::onTransactionErrorCode);
    connect(rpc, &PackageKit::Transaction::finished, this, &XPackageKitTransaction::onGenericTransactionFinished);
#ifdef NEMO_PACKAGE_KIT
    connect(rpc, &PackageKit::Transaction::changed, this, &XPackageKitTransaction::onStatusChanged);
#else
    connect(rpc, &PackageKit::Transaction::statusChanged, this, &XPackageKitTransaction::onStatusChanged);
#endif
}

void XPackageKitTransaction::doRemove(const QString &packageId)
{
    PackageKit::Transaction *rpc = PackageKitBackend::mkRemovePackageTransaction(packageId);
    QObject::connect(rpc, &PackageKit::Transaction::errorCode, [this](PackageKit::Transaction::Error error, const QString &details) {
        setFinishedWithError({{"backend_error", error}, {"backend_details", details}});
        qDebug() << error << details;
    });
    connect(rpc, &PackageKit::Transaction::errorCode, this, &XPackageKitTransaction::onTransactionErrorCode);
    connect(rpc, &PackageKit::Transaction::finished, this, &XPackageKitTransaction::onGenericTransactionFinished);
#ifdef NEMO_PACKAGE_KIT
    connect(rpc, &PackageKit::Transaction::changed, this, &XPackageKitTransaction::onStatusChanged);
#else
    connect(rpc, &PackageKit::Transaction::statusChanged, this, &XPackageKitTransaction::onStatusChanged);
#endif
}

void XPackageKitTransaction::doUpdate(const QString &packageId)
{
    doInstall(packageId);
}
