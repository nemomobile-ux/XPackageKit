#include "XTestUpdate.hpp"
#include <QDateTime>

XTestUpdate::XTestUpdate(const QString &packageName, QObject *parent)
   : XChainTransaction(parent)
{
    setObjectName(QStringLiteral("testChain-%1").arg(QDateTime::currentMSecsSinceEpoch()));
    this->packageName = packageName;

    // set maximum amount of transactions inside chain, used to calculate progress
    setTransactionsMaxCount(10);

    XTransaction *res1 = XTransactionManager::resolve({packageName}, XTransactionNamespace::FilterInstalled);
    connect(res1, &XTransaction::finished, this, &XTestUpdate::onResolveInstalledFinished);
    *this << res1;
}

void XTestUpdate::checkUpdateInstalledDepends()
{
    if (XTransactionManager::isInstalled(updatePkgId)) {
        updateDepends = installedDepends;
    }

    for (QString &pkgId : updateDepends) {
        pkgId = XTransactionManager::packageName(pkgId);
    }

    if (updateDepends.isEmpty()) {
        return;
    }

    XTransaction *res3 = XTransactionManager::resolve(updateDepends, XTransactionNamespace::FilterNewest);
    connect(res3, &XTransaction::finished, this, &XTestUpdate::onResolveInstalledDependsFinished);
    *this << res3;
}

void XTestUpdate::onResolveInstalledFinished(XTransaction *transaction)
{
    if (transaction->results().isEmpty()) {
        xDebug() << "### package not installed:" << packageName;
        return;
    }

    installedPkgId = transaction->results().first().value(QStringLiteral("packageID")).toString();

    *this << XTransactionManager::refreshCache(true);

    XTransaction *dep1 = XTransactionManager::getDepends({installedPkgId}, XTransactionNamespace::FilterInstalled);
    connect(dep1, &XTransaction::finished, this, &XTestUpdate::onDependsInstalledFinished);
    *this << dep1;

    XTransaction *res2 = XTransactionManager::resolve({packageName}, XTransactionNamespace::FilterNewest);
    connect(res2, &XTransaction::finished, this, &XTestUpdate::onResolveAvailableFinished);
    *this << res2;
}

void XTestUpdate::onDependsInstalledFinished(XTransaction *transaction)
{
    for (const QVariantHash &package : transaction->results()) {
        installedDepends << package.value(QStringLiteral("packageID")).toString();
    }
}

void XTestUpdate::onResolveAvailableFinished(XTransaction *transaction)
{
    if (transaction->results().isEmpty()) {
        xDebug() << "### no packages resolved for:" << packageName;
        return;
    }

    updatePkgId = transaction->results().first().value(QStringLiteral("packageID")).toString();

    if (XTransactionManager::isInstalled(updatePkgId)) {
        xDebug() << "### no update candidate for:" << packageName;

        checkUpdateInstalledDepends();

        return;
    }

    XTransaction *dep2 = XTransactionManager::getDepends({updatePkgId}, XTransactionNamespace::FilterNewest);
    connect(dep2, &XTransaction::finished, this, &XTestUpdate::onDependsAvailableFinished);
    *this << dep2;

    XTransaction *down = XTransactionManager::updatePackages({updatePkgId}, XTransactionNamespace::TransactionFlagOnlyDownload);
    connect(down, &XTransaction::finished, this, &XTestUpdate::onUpdateDownloadFinished);
    *this << down;
}

void XTestUpdate::onDependsAvailableFinished(XTransaction *transaction)
{
    for (const QVariantHash &package : transaction->results()) {
        updateDepends << package.value(QStringLiteral("packageID")).toString();
    }

    for (const QString &installedDepend : installedDepends) {
        if (!updateDepends.contains(installedDepend) && XTransactionManager::packageName(installedDepend) != packageName) {
            residualDepends << installedDepend;
            toBeRemovedPkgNames << XTransactionManager::packageName(installedDepend);
        }
    }

    if (!toBeRemovedPkgNames.isEmpty()) {
        emit packagesToBeRemoved(toBeRemovedPkgNames);
    }

    if (updateDepends.isEmpty()) {
        return;
    }

    QStringList updateResolveDepends;

    for (const QString &updateDepend : updateDepends) {
        const QString updateDependName = XTransactionManager::packageName(updateDepend);
        if (!installedDepends.contains(updateDepend) && updateDependName != packageName) {
            pendingDepends << updateDepend;
            toBeInstalledPkgNames << XTransactionManager::packageName(updateDepend);
        }

        updateResolveDepends << updateDependName;
    }

    if (!toBeInstalledPkgNames.isEmpty()) {
        emit packagesToBeInstalled(toBeInstalledPkgNames);
    }
}

void XTestUpdate::onUpdateDownloadFinished(XTransaction *transaction)
{
    if (transaction->results().count() == 0) {
        QVariantHash errorDetails;
        errorDetails[QStringLiteral("chain-error")] = tr("No packages to download");
        setFinishedWithError(errorDetails);
        return;
    }

    *this << XTransactionManager::processTransaction(QStringLiteral("/usr/bin/xpackagekit-verify-cache"),
                                                     QStringList() << QStringLiteral("/var/cache/zypp/packages"));

    XTransaction *upd1 = XTransactionManager::updatePackages({updatePkgId});
    connect(upd1, &XTransaction::finished, this, &XTestUpdate::onUpdatePackageFinished);
    connect(upd1, &XTransaction::progressStatus, this, &XTestUpdate::onUpdatePackageStatusChanged);
    *this << upd1;
}

void XTestUpdate::onUpdatePackageFinished()
{
    if (!residualDepends.isEmpty()) {
        XTransaction *rem1 = XTransactionManager::removePackages(residualDepends);
        connect(rem1, &XTransaction::finished, this, &XTestUpdate::onResidualRemoveFinished);
        *this << rem1;
    }

    checkUpdateInstalledDepends();
}

void XTestUpdate::onUpdatePackageStatusChanged(XTransaction *transaction, const QString &itemName, XTransactionNamespace::TransactionStatus xtstatus, uint progress)
{
    Q_UNUSED(xtstatus)

    if (transaction->status() == XTransactionNamespace::PackageStatusInstall && progress == 100) {
        emit packagesInstalled(QStringList() << XTransactionManager::packageName(itemName));
    }
}

void XTestUpdate::onResolveInstalledDependsFinished(XTransaction *transaction)
{
    QStringList updateResolveDepends;

    for (const QVariantHash &package : transaction->results()) {
        const QString pkgId = package.value(QStringLiteral("packageID")).toString();
        if (!XTransactionManager::isInstalled(pkgId) && !updateResolveDepends.contains(pkgId) && XTransactionManager::packageName(pkgId) != packageName) {
            updateResolveDepends << pkgId;
            toBeUpdatedPkgNames << XTransactionManager::packageName(pkgId);
        }
    }

    if (toBeUpdatedPkgNames.isEmpty()) {
        return;
    }

    emit packagesToBeUpdated(toBeUpdatedPkgNames);

    XTransaction *upd2 = XTransactionManager::updatePackages(updateResolveDepends);
    connect(upd2, &XTransaction::finished, this, &XTestUpdate::onUpdateInstalledDependsFinished);
    *this << upd2;
}

void XTestUpdate::onUpdateInstalledDependsFinished(XTransaction *transaction)
{
    QStringList updateInstalledDepends;

    for (const QVariantHash &package : transaction->results()) {
        const QString pkgId = package.value(QStringLiteral("packageID")).toString();
        const QString pkgName = XTransactionManager::packageName(pkgId);
        if (!XTransactionManager::isInstalled(pkgId) && !updateInstalledDepends.contains(pkgName) && pkgName != packageName) {
            updateInstalledDepends << pkgName;
        }
    }

    if (updateInstalledDepends.isEmpty()) {
        return;
    }

    emit packagesUpdated(updateInstalledDepends);
}

void XTestUpdate::onResidualRemoveFinished(XTransaction *transaction)
{
    QStringList residualRemovedDepends;

    for (const QVariantHash &package : transaction->results()) {
        const QString pkgId = package.value(QStringLiteral("packageID")).toString();
        const QString pkgName = XTransactionManager::packageName(pkgId);
        if (!residualRemovedDepends.contains(pkgName) && pkgName != packageName) {
            residualRemovedDepends << pkgName;
        }
    }

    if (residualRemovedDepends.isEmpty()) {
        return;
    }

    emit packagesRemoved(residualRemovedDepends);
}
