#include "XUpdateTransaction.hpp"
#include "PackageKitBackend.hpp"

XUpdateTransaction::XUpdateTransaction(QObject *parent)
    : XPackageKitTransaction(parent)
{
}

void XUpdateTransaction::onTransactionPackage(PackageKit::Transaction::Info info, const QString &packageID, const QString &summary)
{
    addResult({
                  {QStringLiteral("info"), QVariant::fromValue(info)},
                  {QStringLiteral("packageID"), packageID},
                  {QStringLiteral("summary"), summary},
              });
}

PackageKit::Transaction *XUpdateTransaction::createTransaction()
{
    const QStringList packageIds = requestDetails().value(QStringLiteral("packageIds")).toStringList();
    const XTransactionNamespace::TransactionFlags xflags = requestDetails().value(QStringLiteral("transactionFlags")).value<XTransactionNamespace::TransactionFlags>();
    PackageKit::Transaction *tx = PackageKitBackend::mkUpdatePackagesTransaction(packageIds, toPackageFlags(xflags));
    connect(tx, &PackageKit::Transaction::package, this, &XUpdateTransaction::onTransactionPackage);
    return tx;
}
