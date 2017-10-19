#include "PackageKitBackend.hpp"

#include <Daemon>

PackageKit::Transaction *PackageKitBackend::mkResolveTransaction(const QStringList &packageNames, PackageKit::Transaction::Filters filters)
{
    PackageKit::Transaction *rpc = nullptr;
#ifdef NEMO_PACKAGE_KIT
    rpc = new PackageKit::Transaction();
    rpc->resolve(packageNames, filters);
#else
    rpc = PackageKit::Daemon::resolve(packageNames, filters);
#endif
    return rpc;
}

PackageKit::Transaction *PackageKitBackend::mkSearchNamesTransaction(const QString &searchTerm, PackageKit::Transaction::Filters filters)
{
    PackageKit::Transaction *rpc = nullptr;
#ifdef NEMO_PACKAGE_KIT
    rpc = new PackageKit::Transaction();
    rpc->searchNames(searchTerm, filters);
#else
    rpc = PackageKit::Daemon::searchNames(searchTerm, filters);
#endif
    return rpc;
}

PackageKit::Transaction *PackageKitBackend::mkSearchFilesTransaction(const QString &searchTerm, PackageKit::Transaction::Filters filters)
{
    PackageKit::Transaction *rpc = nullptr;
#ifdef NEMO_PACKAGE_KIT
    rpc = new PackageKit::Transaction();
    rpc->searchFiles(searchTerm, filters);
#else
    rpc = PackageKit::Daemon::searchFiles(searchTerm, filters);
#endif
    return rpc;
}

PackageKit::Transaction *PackageKitBackend::mkInstallPackagesTransaction(const QStringList &packageIds, PackageKit::Transaction::TransactionFlags flags)
{
    PackageKit::Transaction *rpc = nullptr;
#ifdef NEMO_PACKAGE_KIT
    rpc = new PackageKit::Transaction();
    rpc->installPackages(packageIds, flags);
#else
    rpc = PackageKit::Daemon::installPackages(packageIds, flags);
#endif
    return rpc;
}

PackageKit::Transaction *PackageKitBackend::mkInstallFilesTransaction(const QStringList &files)
{
    PackageKit::Transaction *rpc = nullptr;
#ifdef NEMO_PACKAGE_KIT
    rpc = new PackageKit::Transaction();
    rpc->installFiles(files);
#else
    rpc = PackageKit::Daemon::installFiles(files);
#endif
    return rpc;
}

PackageKit::Transaction *PackageKitBackend::mkRepoEnableTransaction(const QString &repoName, bool enabled)
{
    PackageKit::Transaction *rpc = nullptr;
#ifdef NEMO_PACKAGE_KIT
    rpc = new PackageKit::Transaction();
    rpc->repoEnable(repoName, enabled);
#else
    rpc = PackageKit::Daemon::repoEnable(repoName, enabled);
#endif

    return rpc;
}

PackageKit::Transaction *PackageKitBackend::mkRepoSetDataTransaction(const QString &repoId, const QString &parameter, const QString &value)
{
    PackageKit::Transaction *rpc = nullptr;
#ifdef NEMO_PACKAGE_KIT
    rpc = new PackageKit::Transaction();
    rpc->repoSetData(repoId, parameter, value);
#else
    rpc = PackageKit::Daemon::repoSetData(repoId, parameter, value);
#endif
    return rpc;
}

PackageKit::Transaction *PackageKitBackend::mkRefreshRepoTransaction(const QString &repoName, bool force)
{
    return mkRepoSetDataTransaction(repoName, QStringLiteral("refresh-now"), force ? QStringLiteral("true") : QStringLiteral("false"));
}

PackageKit::Transaction *PackageKitBackend::mkRefreshCacheTransaction(bool force)
{
    PackageKit::Transaction *rpc = nullptr;
#ifdef NEMO_PACKAGE_KIT
    rpc = new PackageKit::Transaction();
    rpc->refreshCache(force);
#else
    rpc = PackageKit::Daemon::refreshCache(force);
#endif
    return rpc;
}

PackageKit::Transaction *PackageKitBackend::mkGetUpdatesTransaction(PackageKit::Transaction::Filters filters)
{
    PackageKit::Transaction *rpc = nullptr;
#ifdef NEMO_PACKAGE_KIT
    rpc = new PackageKit::Transaction();
    rpc->getUpdates(filters);
#else
    rpc = PackageKit::Daemon::getUpdates(filters);
#endif
    return rpc;
}

PackageKit::Transaction *PackageKitBackend::mkGetDependsTransaction(const QStringList &packageIds, PackageKit::Transaction::Filters filters)
{
    PackageKit::Transaction *rpc = nullptr;
#ifdef NEMO_PACKAGE_KIT
    rpc = new PackageKit::Transaction();
    rpc->getDepends(packageIds, filters);
#else
    rpc = PackageKit::Daemon::getDepends(packageIds, filters);
#endif
    return rpc;
}

PackageKit::Transaction *PackageKitBackend::mkGetRequiresTransaction(const QStringList &packageIds, PackageKit::Transaction::Filters filters)
{
    PackageKit::Transaction *rpc = nullptr;
#ifdef NEMO_PACKAGE_KIT
    rpc = new PackageKit::Transaction();
    rpc->getRequires(packageIds, filters);
#else
    rpc = PackageKit::Daemon::getRequires(packageIds, filters);
#endif
    return rpc;
}

PackageKit::Transaction *PackageKitBackend::mkGetDetailsTransaction(const QStringList &packageIds)
{
    PackageKit::Transaction *rpc = nullptr;
#ifdef NEMO_PACKAGE_KIT
    rpc = new PackageKit::Transaction();
    rpc->getDetails(packageIds);
#else
    rpc = PackageKit::Daemon::getDetails(packageIds);
#endif
    return rpc;
}

PackageKit::Transaction *PackageKitBackend::mkGetFilesTransaction(const QStringList &packageIds)
{
    PackageKit::Transaction *rpc = nullptr;
#ifdef NEMO_PACKAGE_KIT
    rpc = new PackageKit::Transaction();
    rpc->getFiles(packageIds);
#else
    rpc = PackageKit::Daemon::getFiles(packageIds);
#endif
    return rpc;
}

PackageKit::Transaction *PackageKitBackend::mkUpdatePackagesTransaction(const QStringList &packageIds, PackageKit::Transaction::TransactionFlags flags)
{
    PackageKit::Transaction *rpc = nullptr;
#ifdef NEMO_PACKAGE_KIT
    rpc = new PackageKit::Transaction();
    rpc->updatePackages(packageIds, flags);
#else
    rpc = PackageKit::Daemon::updatePackages(packageIds);
#endif
    return rpc;
}

PackageKit::Transaction *PackageKitBackend::mkRemovePackagesTransaction(const QStringList &packageIds, bool allowDeps, bool autoRemove)
{
    PackageKit::Transaction *rpc = nullptr;
#ifdef NEMO_PACKAGE_KIT
    rpc = new PackageKit::Transaction();
    rpc->removePackages(packageIds, allowDeps, autoRemove);
#else
    rpc = PackageKit::Daemon::removePackages(packageIds, allowDeps, autoRemove);
#endif
    return rpc;
}

PackageKit::Transaction *PackageKitBackend::mkDownloadPackagesTransaction(const QStringList &packageIds, bool storeInCache)
{
    PackageKit::Transaction *rpc = nullptr;
#ifdef NEMO_PACKAGE_KIT
    rpc = new PackageKit::Transaction();
    rpc->downloadPackages(packageIds, storeInCache);
#else
    rpc = PackageKit::Daemon::downloadPackages(packageIds, storeInCache);
#endif
    return rpc;
}
