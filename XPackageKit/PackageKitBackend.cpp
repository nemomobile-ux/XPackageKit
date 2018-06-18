#include "PackageKitBackend.hpp"
#include <Daemon>

PackageKit::Transaction *PackageKitBackend::mkResolveTransaction(const QStringList &packageNames, PackageKit::Transaction::Filters filters)
{
    PackageKit::Transaction *rpc = nullptr;
    rpc = PackageKit::Daemon::resolve(packageNames, filters);
    return rpc;
}

PackageKit::Transaction *PackageKitBackend::mkSearchNamesTransaction(const QString &searchTerm, PackageKit::Transaction::Filters filters)
{
    PackageKit::Transaction *rpc = nullptr;
    rpc = PackageKit::Daemon::searchNames(searchTerm, filters);
    return rpc;
}

PackageKit::Transaction *PackageKitBackend::mkSearchFilesTransaction(const QString &searchTerm, PackageKit::Transaction::Filters filters)
{
    PackageKit::Transaction *rpc = nullptr;
    rpc = PackageKit::Daemon::searchFiles(searchTerm, filters);
    return rpc;
}

PackageKit::Transaction *PackageKitBackend::mkInstallPackagesTransaction(const QStringList &packageIds, PackageKit::Transaction::TransactionFlags flags)
{
    PackageKit::Transaction *rpc = nullptr;
    rpc = PackageKit::Daemon::installPackages(packageIds, flags);
    return rpc;
}

PackageKit::Transaction *PackageKitBackend::mkInstallFilesTransaction(const QStringList &files)
{
    PackageKit::Transaction *rpc = nullptr;
    rpc = PackageKit::Daemon::installFiles(files);
    return rpc;
}

PackageKit::Transaction *PackageKitBackend::mkRepoEnableTransaction(const QString &repoName, bool enabled)
{
    PackageKit::Transaction *rpc = nullptr;
    rpc = PackageKit::Daemon::repoEnable(repoName, enabled);

    return rpc;
}

PackageKit::Transaction *PackageKitBackend::mkRepoSetDataTransaction(const QString &repoId, const QString &parameter, const QString &value)
{
    PackageKit::Transaction *rpc = nullptr;
    rpc = PackageKit::Daemon::repoSetData(repoId, parameter, value);
    return rpc;
}

PackageKit::Transaction *PackageKitBackend::mkRefreshRepoTransaction(const QString &repoName, bool force)
{
    return mkRepoSetDataTransaction(repoName, QStringLiteral("refresh-now"), force ? QStringLiteral("true") : QStringLiteral("false"));
}

PackageKit::Transaction *PackageKitBackend::mkRefreshCacheTransaction(bool force)
{
    PackageKit::Transaction *rpc = nullptr;
    rpc = PackageKit::Daemon::refreshCache(force);
    return rpc;
}

PackageKit::Transaction *PackageKitBackend::mkGetUpdatesTransaction(PackageKit::Transaction::Filters filters)
{
    PackageKit::Transaction *rpc = nullptr;
    rpc = PackageKit::Daemon::getUpdates(filters);
    return rpc;
}

PackageKit::Transaction *PackageKitBackend::mkGetDependsTransaction(const QStringList &packageIds, PackageKit::Transaction::Filters filters)
{
    PackageKit::Transaction *rpc = nullptr;
    rpc = PackageKit::Daemon::dependsOn(packageIds, filters);
    return rpc;
}

PackageKit::Transaction *PackageKitBackend::mkGetRequiresTransaction(const QStringList &packageIds, PackageKit::Transaction::Filters filters)
{
    PackageKit::Transaction *rpc = nullptr;
    rpc = PackageKit::Daemon::requiredBy(packageIds, filters);
    return rpc;
}

PackageKit::Transaction *PackageKitBackend::mkGetDetailsTransaction(const QStringList &packageIds)
{
    PackageKit::Transaction *rpc = nullptr;
    rpc = PackageKit::Daemon::getDetails(packageIds);
    return rpc;
}

PackageKit::Transaction *PackageKitBackend::mkGetFilesTransaction(const QStringList &packageIds)
{
    PackageKit::Transaction *rpc = nullptr;
    rpc = PackageKit::Daemon::getFiles(packageIds);
    return rpc;
}

PackageKit::Transaction *PackageKitBackend::mkGetPackagesTransaction(PackageKit::Transaction::Filters filters)
{
    PackageKit::Transaction *rpc = nullptr;
    rpc = PackageKit::Daemon::getPackages(filters);
    return rpc;
}

PackageKit::Transaction *PackageKitBackend::mkUpdatePackagesTransaction(const QStringList &packageIds, PackageKit::Transaction::TransactionFlags flags)
{
    PackageKit::Transaction *rpc = nullptr;
    rpc = PackageKit::Daemon::updatePackages(packageIds);
    return rpc;
}

PackageKit::Transaction *PackageKitBackend::mkRemovePackagesTransaction(const QStringList &packageIds, bool allowDeps, bool autoRemove)
{
    PackageKit::Transaction *rpc = nullptr;
    rpc = PackageKit::Daemon::removePackages(packageIds, allowDeps, autoRemove);
    return rpc;
}

PackageKit::Transaction *PackageKitBackend::mkDownloadPackagesTransaction(const QStringList &packageIds, bool storeInCache)
{
    PackageKit::Transaction *rpc = nullptr;
    rpc = PackageKit::Daemon::downloadPackages(packageIds, storeInCache);
    return rpc;
}
