#include "XRemoveTransaction.hpp"
#include "PackageKitBackend.hpp"

XRemoveTransaction::XRemoveTransaction(QObject *parent)
    : XPackageKitTransaction(parent)
{
}

void XRemoveTransaction::onTransactionPackage(PackageKit::Transaction::Info info, const QString &packageID, const QString &summary)
{
    addResult({
                  {QStringLiteral("info"), QVariant::fromValue(info)},
                  {QStringLiteral("packageID"), packageID},
                  {QStringLiteral("summary"), summary},
              });
}

PackageKit::Transaction *XRemoveTransaction::createTransaction()
{
    const QStringList packageNames = requestDetails().value(QStringLiteral("packageNames")).toStringList();
    const bool allowDeps = requestDetails().value(QStringLiteral("allowDeps"), false).toBool();
    const bool autoRemove = requestDetails().value(QStringLiteral("autoRemove"), false).toBool();
    PackageKit::Transaction *tx = PackageKitBackend::mkRemovePackagesTransaction(packageNames, allowDeps, autoRemove);
    connect(tx, &PackageKit::Transaction::package, this, &XRemoveTransaction::onTransactionPackage);
    return tx;
}
