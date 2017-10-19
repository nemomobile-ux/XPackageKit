#include "XGetDependsTransaction.hpp"
#include "PackageKitBackend.hpp"

XGetDependsTransaction::XGetDependsTransaction(QObject *parent)
    : XPackageKitTransaction(parent)
{
}

void XGetDependsTransaction::onTransactionPackage(PackageKit::Transaction::Info info, const QString &packageID, const QString &summary)
{
    addResult({
                  {QStringLiteral("info"), QVariant::fromValue(info)},
                  {QStringLiteral("packageID"), packageID},
                  {QStringLiteral("summary"), summary},
              });
}

PackageKit::Transaction *XGetDependsTransaction::createTransaction()
{
    const QStringList packageIds = requestDetails().value(QStringLiteral("packageIds")).toStringList();
    const XTransactionNamespace::Filters xfilters = requestDetails().value(QStringLiteral("filters")).value<XTransactionNamespace::Filters>();
    const PackageKit::Transaction::Filters pfilters = toPackageFilters(xfilters);
    PackageKit::Transaction *tx = PackageKitBackend::mkGetDependsTransaction(packageIds, pfilters);
    connect(tx, &PackageKit::Transaction::package, this, &XGetDependsTransaction::onTransactionPackage);
    return tx;
}
