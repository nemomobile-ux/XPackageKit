#include "XGetPackagesTransaction.hpp"
#include "PackageKitBackend.hpp"

XGetPackagesTransaction::XGetPackagesTransaction(const QString &newObjectName, QObject *parent)
    : XPackageKitTransaction(newObjectName, parent)
{
}

void XGetPackagesTransaction::onTransactionPackage(PackageKit::Transaction::Info info, const QString &packageID, const QString &summary)
{
    addResult({
                  {QStringLiteral("info"), QVariant::fromValue(info)},
                  {QStringLiteral("packageID"), packageID},
                  {QStringLiteral("summary"), summary},
              });
}

PackageKit::Transaction *XGetPackagesTransaction::createTransaction()
{
    const XTransactionNamespace::Filters xfilters = requestDetails().value(QStringLiteral("filters")).value<XTransactionNamespace::Filters>();
    const PackageKit::Transaction::Filters pfilters = toPackageFilters(xfilters);
    PackageKit::Transaction *tx = PackageKitBackend::mkGetPackagesTransaction(pfilters);
    connect(tx, &PackageKit::Transaction::package, this, &XGetPackagesTransaction::onTransactionPackage);
    return tx;
}
