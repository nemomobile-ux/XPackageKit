#include "XInstallTransaction.hpp"
#include "PackageKitBackend.hpp"

XInstallTransaction::XInstallTransaction(QObject *parent)
    : XPackageKitTransaction(parent)
{

}

void XInstallTransaction::onTransactionPackage(PackageKit::Transaction::Info info, const QString &packageID, const QString &summary)
{
    addResult({
                  {QStringLiteral("info"), QVariant::fromValue(info)},
                  {QStringLiteral("packageID"), packageID},
                  {QStringLiteral("summary"), summary},
              });
}

PackageKit::Transaction *XInstallTransaction::createTransaction()
{
    const QStringList packageNames = requestDetails().value(QStringLiteral("packageNames")).toStringList();
    const XTransactionNamespace::TransactionFlags xflags = requestDetails().value(QStringLiteral("transactionFlags")).value<XTransactionNamespace::TransactionFlags>();
    PackageKit::Transaction *tx = PackageKitBackend::mkInstallPackagesTransaction(packageNames, toPackageFlags(xflags));
    connect(tx, &PackageKit::Transaction::package, this, &XInstallTransaction::onTransactionPackage);
    return tx;
}
