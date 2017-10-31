#include "XDownloadTransaction.hpp"
#include "PackageKitBackend.hpp"

XDownloadTransaction::XDownloadTransaction(QObject *parent)
    : XPackageKitTransaction(parent)
{
}

void XDownloadTransaction::onTransactionPackage(PackageKit::Transaction::Info info, const QString &packageID, const QString &summary)
{
    addResult({
                  {QStringLiteral("info"), QVariant::fromValue(info)},
                  {QStringLiteral("packageID"), packageID},
                  {QStringLiteral("summary"), summary},
              });
}

PackageKit::Transaction *XDownloadTransaction::createTransaction()
{
    const QStringList packageIds = requestDetails().value(QStringLiteral("packageIds")).toStringList();
    const bool storeInCache = requestDetails().value(QStringLiteral("storeInCache"), false).toBool();
    PackageKit::Transaction *tx = PackageKitBackend::mkDownloadPackagesTransaction(packageIds, storeInCache);
    connect(tx, &PackageKit::Transaction::package, this, &XDownloadTransaction::onTransactionPackage);
    return tx;
}

