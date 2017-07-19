#include "PackageKitBackend.hpp"

#include <Daemon>

PackageKit::Transaction *PackageKitBackend::mkResolveTransaction(const QString &resolveTerm, PackageKit::Transaction::Filters filters)
{
    PackageKit::Transaction *rpc = nullptr;
#ifdef NEMO_PACKAGE_KIT
    rpc = new PackageKit::Transaction();
    rpc->resolve(resolveTerm, filters);
#else
    rpc = PackageKit::Daemon::resolve(resolveTerm, filters);
#endif
    return rpc;
}

PackageKit::Transaction *PackageKitBackend::mkSearchNameTransaction(const QString &searchTerm, PackageKit::Transaction::Filters filters)
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

PackageKit::Transaction *PackageKitBackend::mkInstallPackageTransaction(const QString &packageID, PackageKit::Transaction::TransactionFlags flags)
{
    PackageKit::Transaction *rpc = nullptr;
#ifdef NEMO_PACKAGE_KIT
    rpc = new PackageKit::Transaction();
    rpc->installPackage(packageID, flags);
#else
    rpc = PackageKit::Daemon::installPackage(packageID, flags);
#endif
    return rpc;
}

PackageKit::Transaction *PackageKitBackend::mkRemovePackageTransaction(const QString &packageID, bool allowDeps, bool autoRemove, PackageKit::Transaction::TransactionFlags flags)
{
    PackageKit::Transaction *rpc = nullptr;
#ifdef NEMO_PACKAGE_KIT
    rpc = new PackageKit::Transaction();
    rpc->removePackage(packageID, allowDeps, autoRemove, flags);
#else
    rpc = PackageKit::Daemon::removePackage(packageID, allowDeps, autoRemove, flags);
#endif
    return rpc;
}

PackageKit::Transaction *PackageKitBackend::mkGetFilesTransaction(const QString &packageIDs)
{
    PackageKit::Transaction *rpc = nullptr;
#ifdef NEMO_PACKAGE_KIT
    rpc = new PackageKit::Transaction();
    rpc->getFiles(packageIDs);
#else
    rpc = PackageKit::Daemon::getFiles(packageIDs);
#endif

    return rpc;
}

PackageKit::Transaction *PackageKitBackend::mkSetRepoEnabledTransaction(const QString &repoName, bool enabled)
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

PackageKit::Transaction *PackageKitBackend::mkRefreshRepoTransaction(const QString &repoName)
{
    PackageKit::Transaction *rpc = nullptr;
#ifdef NEMO_PACKAGE_KIT
    rpc = new PackageKit::Transaction();
    rpc->repoSetData(repoName, QStringLiteral("refresh-now"), QStringLiteral("true"));
#else
    rpc = PackageKit::Daemon::repoSetData(repoName, QStringLiteral("refresh-now"), QStringLiteral("true"));
#endif
    return rpc;
}
