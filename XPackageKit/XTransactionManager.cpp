#include "XTransactionManager.hpp"
#include "XChainTransaction.hpp"

#include "XRefreshTransaction.hpp"
#include "XInstallTransaction.hpp"
#include "XRemoveTransaction.hpp"
#include "XSsuTransaction.hpp"
#include "XGetUpdatesTransaction.hpp"
#include "XResolveTransaction.hpp"
#include "XGetDependsTransaction.hpp"
#include "XUpdateTransaction.hpp"
#include "XDownloadTransaction.hpp"
#include "XProcessTransaction.hpp"

/*!
    \class XTransactionManager
    \brief Manager for creating transactions
    \inmodule XPackageKit
*/

static QObject *s_defaultParent = nullptr;
static bool s_autodelete = true;

template<typename T>
T *createTransaction(const QString &objectName, QObject *parent)
{
    if (parent == nullptr) {
        parent = XTransactionManager::defaultParent();
    }

    T *t = new T(parent);
    t->setObjectName(QStringLiteral("%1-%2").arg(objectName).arg(QDateTime::currentMSecsSinceEpoch()));
    if (XTransactionManager::isAutodelete()) {
        QObject::connect(t, &XTransaction::finished, t, &QObject::deleteLater);
    }
    return t;
}

/*!
    \fn void XTransactionManager::setDefaultParent(QObject *parent)

    Set default parent to \a parent
*/

void XTransactionManager::setDefaultParent(QObject *parent)
{
    s_defaultParent = parent;
}

/*!
    \fn QObject *XTransactionManager::defaultParent()

    Returns default parent
*/

QObject *XTransactionManager::defaultParent()
{
    return s_defaultParent;
}

/*!
    \fn void XTransactionManager::setAutodelete(bool autodelete)

    Enables transactions autodelete if \a autodelete is \c true;
    otherwise transactions should be deleted manually.
*/

void XTransactionManager::setAutodelete(bool autodelete)
{
    s_autodelete = autodelete;
}

/*!
    \fn bool XTransactionManager::isAutodelete()

    Returns \c true if autodelete is enabled \c; otherwise returns false.
*/

bool XTransactionManager::isAutodelete()
{
    return s_autodelete;
}

/*!
    \fn XTransaction *XTransactionManager::addRepository(const QString &repoName, const QVariantHash &details, QObject *parent)

    \a repoName \a details \a parent

    Returns transaction.
*/

XTransaction *XTransactionManager::addRepository(const QString &repoName, const QVariantHash &details, QObject *parent)
{
    XTransaction *t = createTransaction<XSsuTransaction>(QStringLiteral("addRepository"), parent);
    QVariantHash detailsEx = details;
    detailsEx.insert(QStringLiteral("action"), QVariant::fromValue(XSsuTransaction::SsuRepoAction::Add));
    detailsEx.insert(QStringLiteral("repoName"), repoName);
    t->setRequestDetails(detailsEx);
    return t;
}

/*!
    \fn XTransaction *XTransactionManager::removeRepository(const QString &repoName, QObject *parent)

    \a repoName \a parent

    Returns transaction.
*/

XTransaction *XTransactionManager::removeRepository(const QString &repoName, QObject *parent)
{
    XTransaction *t = createTransaction<XSsuTransaction>(QStringLiteral("removeRepository"), parent);
    t->setRequestDetails({
                             {QStringLiteral("repoName"), repoName},
                             {QStringLiteral("action"), QVariant::fromValue(XSsuTransaction::SsuRepoAction::Remove)},
                         });
    return t;
}

/*!
    \fn XTransaction *XTransactionManager::setRepositoryEnabled(const QString &repoName, bool enable, QObject *parent)

    \a repoName \a enable \a parent

    Returns transaction.
*/

XTransaction *XTransactionManager::setRepositoryEnabled(const QString &repoName, bool enable, QObject *parent)
{
    XTransaction *t = createTransaction<XSsuTransaction>(QStringLiteral("setRepositoryEnabled"), parent);
    const XSsuTransaction::SsuRepoAction action = enable
            ? XSsuTransaction::SsuRepoAction::Enable
            : XSsuTransaction::SsuRepoAction::Disable;
    t->setRequestDetails({
                             {QStringLiteral("repoName"), repoName},
                             {QStringLiteral("action"), QVariant::fromValue(action)},
                         });
    return t;
}

/*!
    \fn XTransaction *XTransactionManager::refreshRepository(const QString &repoName, bool force, QObject *parent)

    \a repoName \a force \a parent

    Returns transaction.
*/

XTransaction *XTransactionManager::refreshRepository(const QString &repoName, bool force, QObject *parent)
{
    XTransaction *t = createTransaction<XRefreshTransaction>(QStringLiteral("refreshRepository"), parent);
    t->setRequestDetails({
                             {QStringLiteral("repoName"), repoName},
                             {QStringLiteral("force"), force},
                         });
    return t;
}

/*!
    \fn XTransaction *XTransactionManager::refreshCache(bool force, QObject *parent)

    \a force \a parent

    Returns transaction.
*/

XTransaction *XTransactionManager::refreshCache(bool force, QObject *parent)
{
    XTransaction *t = createTransaction<XRefreshTransaction>(QStringLiteral("refreshCache"), parent);
    t->setRequestDetails({{QStringLiteral("force"), force}});
    return t;
}

/*!
    \fn XTransaction *XTransactionManager::installPackages(const QStringList &packageNames, XTransactionNamespace::TransactionFlags transactionFlags, QObject *parent)

    \a packageNames \a transactionFlags \a parent

    Returns transaction.
*/

XTransaction *XTransactionManager::installPackages(const QStringList &packageNames, XTransactionNamespace::TransactionFlags transactionFlags, QObject *parent)
{
    XTransaction *t = createTransaction<XInstallTransaction>(QStringLiteral("installPackages"), parent);
    t->setRequestDetails({{QStringLiteral("packageNames"), packageNames},
                          {QStringLiteral("transactionFlags"), QVariant::fromValue(transactionFlags)}});
    return t;
}

/*!
    \fn XTransaction *XTransactionManager::removePackages(const QStringList &packageNames, QObject *parent)

    \a packageNames \a parent

   Create a transaction to remove a packages \a packageNames.
   Well-known extra request details are booleans allowDeps and autoRemove.

   Returns the transaction for given action and parameters.
*/

XTransaction *XTransactionManager::removePackages(const QStringList &packageNames, QObject *parent)
{
    XTransaction *t = createTransaction<XRemoveTransaction>(QStringLiteral("removePackages"), parent);
    t->setRequestDetails({{QStringLiteral("packageNames"), packageNames }});
    return t;
}

/*!
    \fn XTransaction *XTransactionManager::getUpdates(XTransactionNamespace::Filters filters, QObject *parent)

    \a filters \a parent

    Returns transaction.
*/

XTransaction *XTransactionManager::getUpdates(XTransactionNamespace::Filters filters, QObject *parent)
{
    XTransaction *t = createTransaction<XGetUpdatesTransaction>(QStringLiteral("getUpdates"), parent);
    t->setRequestDetails({{QStringLiteral("filters"), QVariant::fromValue(filters) }});
    return t;
}

/*!
    \fn XTransaction *XTransactionManager::resolve(const QStringList &packageNames, XTransactionNamespace::Filters filters, QObject *parent)

    \a packageNames \a filters \a parent

    Returns transaction.
*/

XTransaction *XTransactionManager::resolve(const QStringList &packageNames, XTransactionNamespace::Filters filters, QObject *parent)
{
    XTransaction *t = createTransaction<XResolveTransaction>(QStringLiteral("resolve"), parent);
    t->setRequestDetails({{QStringLiteral("packageNames"), packageNames },
                          {QStringLiteral("filters"), QVariant::fromValue(filters) }});
    return t;
}

/*!
    \fn XTransaction *XTransactionManager::getDepends(const QStringList &packageIds, XTransactionNamespace::Filters filters, QObject *parent)

    \a packageIds \a filters \a parent

    Returns transaction.
*/

XTransaction *XTransactionManager::getDepends(const QStringList &packageIds, XTransactionNamespace::Filters filters, QObject *parent)
{
    XTransaction *t = createTransaction<XGetDependsTransaction>(QStringLiteral("getDepends"), parent);
    t->setRequestDetails({{QStringLiteral("packageIds"), packageIds },
                          {QStringLiteral("filters"), QVariant::fromValue(filters) }});
    return t;
}

/*!
    \fn XTransaction *XTransactionManager::updatePackages(const QStringList &packageIds, XTransactionNamespace::TransactionFlags transactionFlags, QObject *parent)

    \a packageIds \a transactionFlags \a parent

    Returns transaction.
*/

XTransaction *XTransactionManager::updatePackages(const QStringList &packageIds, XTransactionNamespace::TransactionFlags transactionFlags, QObject *parent)
{
    XTransaction *t = createTransaction<XUpdateTransaction>(QStringLiteral("updatePackages"), parent);
    t->setRequestDetails({{QStringLiteral("packageIds"), packageIds},
                          {QStringLiteral("transactionFlags"), QVariant::fromValue(transactionFlags)}});
    return t;
}

/*!
    \fn XTransaction *XTransactionManager::downloadPackages(const QStringList &packageIds, bool storeInCache, QObject *parent)

    \a packageIds \a storeInCache \a parent

    Returns transaction.
*/

XTransaction *XTransactionManager::downloadPackages(const QStringList &packageIds, bool storeInCache, QObject *parent)
{
    XTransaction *t = createTransaction<XDownloadTransaction>(QStringLiteral("downloadPackages"), parent);
    t->setRequestDetails({{QStringLiteral("packageIds"), packageIds},
                          {QStringLiteral("storeInCache"), QVariant::fromValue(storeInCache)}});
    return t;
}

/*!
    \fn XTransaction *XTransactionManager::processTransaction(const QString &program, const QStringList &arguments, QObject *parent)

    \a program \a arguments \a parent

    Returns transaction.
*/

XTransaction *XTransactionManager::processTransaction(const QString &program, const QStringList &arguments, QObject *parent)
{
    XTransaction *t = createTransaction<XProcessTransaction>(QStringLiteral("processTransaction"), parent);
    t->setRequestDetails({{QStringLiteral("program"), program},
                          {QStringLiteral("arguments"), arguments}});
    return t;
}

/*!
    \fn XChainTransaction *XTransactionManager::createChain(const QVector<XTransaction *> &transactions, QObject *parent)

    \a transactions \a parent

    Returns chain transaction.
*/

XChainTransaction *XTransactionManager::createChain(const QVector<XTransaction *> &transactions, QObject *parent)
{
    XChainTransaction *t = createTransaction<XChainTransaction>(QStringLiteral("chain"), parent);
    for (XTransaction *child : transactions) {
        *t << child;
    }
    return t;
}

/*!
    \fn QString XTransactionManager::packageName(const QString &packageId)

    \a packageId

    Returns package name.
*/

QString XTransactionManager::packageName(const QString &packageId)
{
    return XPackageKitTransaction::packageName(packageId);
}

/*!
    \fn QString XTransactionManager::packageVersion(const QString &packageId)

    \a packageId

    Returns package version.
*/

QString XTransactionManager::packageVersion(const QString &packageId)
{
    return XPackageKitTransaction::packageVersion(packageId);
}

/*!
    \fn QString XTransactionManager::packageArch(const QString &packageId)

    \a packageId

    Returns package architecture.
*/

QString XTransactionManager::packageArch(const QString &packageId)
{
    return XPackageKitTransaction::packageArch(packageId);
}

/*!
    \fn QString XTransactionManager::packageData(const QString &packageId)

    \a packageId

    Returns package data.
*/

QString XTransactionManager::packageData(const QString &packageId)
{
    return XPackageKitTransaction::packageData(packageId);
}

/*!
    \fn bool XTransactionManager::isInstalled(const QString &packageId)

    \a packageId

    Returns \c true if packageId is installed, \c false otherwise.
*/

bool XTransactionManager::isInstalled(const QString &packageId)
{
    return XTransactionManager::packageData(packageId) == QLatin1String("installed");
}
