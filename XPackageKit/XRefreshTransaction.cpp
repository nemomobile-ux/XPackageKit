#include "XRefreshTransaction.hpp"
#include "PackageKitBackend.hpp"

XRefreshTransaction::XRefreshTransaction(QObject *parent)
    : XPackageKitTransaction(parent)
{
}

PackageKit::Transaction *XRefreshTransaction::createTransaction()
{
    const QString repoName = requestDetails().value(QStringLiteral("repoName")).toString();
    const bool force = requestDetails().value(QStringLiteral("force")).toBool();
    if (repoName.isEmpty()) {
        return PackageKitBackend::mkRefreshCacheTransaction(force);
    } else {
        return PackageKitBackend::mkRefreshRepoTransaction(repoName, force);
    }
}
