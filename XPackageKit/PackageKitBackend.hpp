#ifndef PACKAGEKITBACKEND_HPP
#define PACKAGEKITBACKEND_HPP

#include <QObject>

#include <Transaction>

class PackageKitBackend : public QObject
{
    Q_OBJECT
public:
    static PackageKit::Transaction *mkResolveTransaction(const QStringList &packageNames, PackageKit::Transaction::Filters filters = PackageKit::Transaction::FilterNone);
    static PackageKit::Transaction *mkSearchNamesTransaction(const QString &searchTerm, PackageKit::Transaction::Filters filters = PackageKit::Transaction::FilterNone);
    static PackageKit::Transaction *mkSearchFilesTransaction(const QString &searchTerm, PackageKit::Transaction::Filters filters = PackageKit::Transaction::FilterNone);
    static PackageKit::Transaction *mkInstallPackagesTransaction(const QStringList &packageIds, PackageKit::Transaction::TransactionFlags flags = PackageKit::Transaction::TransactionFlagNone);
    static PackageKit::Transaction *mkInstallFilesTransaction(const QStringList &files);
    static PackageKit::Transaction *mkRepoEnableTransaction(const QString &repoName, bool enabled);
    static PackageKit::Transaction *mkRepoSetDataTransaction(const QString &repoId, const QString &parameter, const QString &value);
    static PackageKit::Transaction *mkRefreshRepoTransaction(const QString &repoName, bool force = false);
    static PackageKit::Transaction *mkRefreshCacheTransaction(bool force = false);
    static PackageKit::Transaction *mkGetUpdatesTransaction(PackageKit::Transaction::Filters filters = PackageKit::Transaction::FilterNone);
    static PackageKit::Transaction *mkGetDependsTransaction(const QStringList &packageIds, PackageKit::Transaction::Filters filters = PackageKit::Transaction::FilterNone);
    static PackageKit::Transaction *mkGetRequiresTransaction(const QStringList &packageIds, PackageKit::Transaction::Filters filters = PackageKit::Transaction::FilterNone);
    static PackageKit::Transaction *mkGetDetailsTransaction(const QStringList &packageIds);
    static PackageKit::Transaction *mkGetFilesTransaction(const QStringList &packageIds);
    static PackageKit::Transaction *mkUpdatePackagesTransaction(const QStringList &packageIds, PackageKit::Transaction::TransactionFlags flags = PackageKit::Transaction::TransactionFlagNone);
    static PackageKit::Transaction *mkRemovePackagesTransaction(const QStringList &packageIds, bool allowDeps = false, bool autoRemove = false);
    static PackageKit::Transaction *mkDownloadPackagesTransaction(const QStringList &packageIds, bool storeInCache = false);

};

#endif // PACKAGEKITBACKEND_HPP
