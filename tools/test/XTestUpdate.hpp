#ifndef XTEST_HPP
#define XTEST_HPP

#include <QObject>

#include "XPackageKit/XTransaction.hpp"
#include "XPackageKit/XChainTransaction.hpp"
#include "XPackageKit/XTransactionManager.hpp"

class XTestUpdate : public XChainTransaction
{
    Q_OBJECT
public:
    explicit XTestUpdate(const QString &packageName, QObject *parent = 0);

private:
    void checkUpdateInstalledDepends();

    QString packageName;
    QString installedPkgId;
    QString updatePkgId;

    QStringList installedDepends;
    QStringList updateDepends;
    QStringList residualDepends;
    QStringList pendingDepends;

    QStringList toBeInstalledPkgNames;
    QStringList toBeUpdatedPkgNames;
    QStringList toBeRemovedPkgNames;

private slots:
    void onResolveInstalledFinished(XTransaction *transaction);
    void onDependsInstalledFinished(XTransaction *transaction);
    void onResolveAvailableFinished(XTransaction *transaction);
    void onDependsAvailableFinished(XTransaction *transaction);
    void onUpdateDownloadFinished(XTransaction *transaction);
    void onUpdatePackageFinished();
    void onUpdatePackageStatusChanged(XTransaction *transaction, const QString &itemName, XTransactionNamespace::TransactionStatus status, uint progress);
    void onResolveInstalledDependsFinished(XTransaction *transaction);
    void onUpdateInstalledDependsFinished(XTransaction *transaction);
    void onResidualRemoveFinished(XTransaction *transaction);

signals:
    void packagesToBeInstalled(const QStringList &packageNames);
    void packagesInstalled(const QStringList &packageNames);
    void packagesToBeUpdated(const QStringList &packageNames);
    void packagesUpdated(const QStringList &packageNames);
    void packagesToBeRemoved(const QStringList &packageNames);
    void packagesRemoved(const QStringList &packageNames);

};

#endif // XTEST_HPP
