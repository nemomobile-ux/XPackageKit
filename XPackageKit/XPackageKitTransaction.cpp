#include "XPackageKitTransaction.hpp"
#include <map>

#include <QDebug>

XPackageKitTransaction::XPackageKitTransaction(QObject *parent)
    : XTransaction(parent)
{
    setStatus(XTransactionNamespace::PackageStatusUnknown);
}

PackageKit::Transaction::Filters XPackageKitTransaction::toPackageFilters(XTransactionNamespace::Filters txFilters)
{
    PackageKit::Transaction::Filters filter;

    std::map<XTransactionNamespace::Filter, PackageKit::Transaction::Filter> f = {
        { XTransactionNamespace::FilterNone        , PackageKit::Transaction::FilterNone         },
        { XTransactionNamespace::FilterInstalled   , PackageKit::Transaction::FilterInstalled    },
        { XTransactionNamespace::FilterNotInstalled, PackageKit::Transaction::FilterNotInstalled },
        { XTransactionNamespace::FilterBasename    , PackageKit::Transaction::FilterBasename     },
        { XTransactionNamespace::FilterNotBasename , PackageKit::Transaction::FilterNotBasename  },
        { XTransactionNamespace::FilterNewest      , PackageKit::Transaction::FilterNewest       },
        { XTransactionNamespace::FilterNotNewest   , PackageKit::Transaction::FilterNotNewest    },
    };

    for (auto of : f) {
        if (txFilters & of.first) {
            filter |= of.second;
        }
    }

    return filter;
}

PackageKit::Transaction::TransactionFlags XPackageKitTransaction::toPackageFlags(XTransactionNamespace::TransactionFlags txFlags)
{
    PackageKit::Transaction::TransactionFlags flag;

    std::map<XTransactionNamespace::TransactionFlag, PackageKit::Transaction::TransactionFlag> f = {
        { XTransactionNamespace::TransactionFlagNone        , PackageKit::Transaction::TransactionFlagNone          },
        { XTransactionNamespace::TransactionFlagOnlyTrusted , PackageKit::Transaction::TransactionFlagOnlyTrusted   },
        { XTransactionNamespace::TransactionFlagSimulate    , PackageKit::Transaction::TransactionFlagSimulate      },
        { XTransactionNamespace::TransactionFlagOnlyDownload, PackageKit::Transaction::TransactionFlagOnlyDownload  },
    };

    for (auto of : f) {
        if (txFlags & of.first) {
            flag |= of.second;
        }
    }

    return flag;
}

PackageKit::Transaction::Status XPackageKitTransaction::packageStatus(XTransactionNamespace::TransactionStatus txStatus)
{
    return static_cast<PackageKit::Transaction::Status>(txStatus);
}

XTransactionNamespace::TransactionStatus XPackageKitTransaction::transactionStatus(PackageKit::Transaction::Status pkStatus)
{
    return static_cast<XTransactionNamespace::TransactionStatus>(pkStatus);
}

QString XPackageKitTransaction::packageName(const QString &pkgId)
{
    return PackageKit::Transaction::packageName(pkgId);
}

QString XPackageKitTransaction::packageVersion(const QString &pkgId)
{
    return PackageKit::Transaction::packageVersion(pkgId);
}

QString XPackageKitTransaction::packageArch(const QString &pkgId)
{
    return PackageKit::Transaction::packageArch(pkgId);
}

QString XPackageKitTransaction::packageData(const QString &pkgId)
{
    return PackageKit::Transaction::packageData(pkgId);
}

void XPackageKitTransaction::onTransactionErrorCode(PackageKit::Transaction::Error error, const QString &details)
{
    xDebug() << error << details;
    m_errors.append({ error, details });
}

void XPackageKitTransaction::onTransactionFinished(PackageKit::Transaction::Exit exitStatus, uint runtime)
{
    xDebug() << exitStatus << runtime;
    if (exitStatus == PackageKit::Transaction::ExitSuccess) {
        setFinished();
    } else {
        QVariantHash details = {
            {QStringLiteral("backend_exitCode"), exitStatus},
            {QStringLiteral("backend_runTime"), runtime}
        };

        if (!m_errors.isEmpty()) {
            details[QStringLiteral("backend_errorCode")] = static_cast<int>(m_errors.last().error);
            details[QStringLiteral("backend_errorDetails")] = m_errors.last().details;
        }


        setFinishedWithError(details);
    }
}

void XPackageKitTransaction::onTransactionMessage(PackageKit::Transaction::Message type, const QString &message)
{
    xDebug() << type << message;
}

void XPackageKitTransaction::onTransactionItemProgress(const QString &itemID, PackageKit::Transaction::Status pkStatus, uint percentage)
{
    xDebug() << itemID << pkStatus << percentage;
    emit progressStatus(this, itemID, XPackageKitTransaction::transactionStatus(pkStatus), percentage);
}

void XPackageKitTransaction::onTransactionStatusChanged()
{
    PackageKit::Transaction *tx = qobject_cast<PackageKit::Transaction*>(sender());
    if (!tx) {
        return;
    }
    setProgress(tx->percentage());
    setStatus(static_cast<XTransactionNamespace::TransactionStatus>(tx->status()));
    xDebug() << "percentage:" << progress() << "status:" << status();
}

void XPackageKitTransaction::startEvent()
{
    PackageKit::Transaction *tx = createTransaction();
    connect(tx, &PackageKit::Transaction::finished, this, &XPackageKitTransaction::onTransactionFinished);
    connect(tx, &PackageKit::Transaction::errorCode, this, &XPackageKitTransaction::onTransactionErrorCode);
    connect(tx, &PackageKit::Transaction::message, this, &XPackageKitTransaction::onTransactionMessage);
    connect(tx, &PackageKit::Transaction::itemProgress, this, &XPackageKitTransaction::onTransactionItemProgress);
#ifdef NEMO_PACKAGE_KIT
    connect(tx, &PackageKit::Transaction::changed, this, &XPackageKitTransaction::onTransactionStatusChanged);
#else
    connect(tx, &PackageKit::Transaction::statusChanged, this, &XPackageKitTransaction::onTransactionStatusChanged);
#endif
}
