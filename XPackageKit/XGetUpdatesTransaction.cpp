#include "XGetUpdatesTransaction.hpp"
#include "PackageKitBackend.hpp"

XGetUpdatesTransaction::XGetUpdatesTransaction(QObject *parent)
    : XPackageKitTransaction(parent)
{

}

void XGetUpdatesTransaction::onTransactionPackage(PackageKit::Transaction::Info info, const QString &packageID, const QString &summary)
{
    addResult({
                  {QStringLiteral("info"), QVariant::fromValue(info)},
                  {QStringLiteral("packageID"), packageID},
                  {QStringLiteral("summary"), summary},
              });
}

PackageKit::Transaction *XGetUpdatesTransaction::createTransaction()
{
    const XTransactionNamespace::Filters xfilters = requestDetails().value(QStringLiteral("filters")).value<XTransactionNamespace::Filters>();
    const PackageKit::Transaction::Filters pfilters = toPackageFilters(xfilters);
    PackageKit::Transaction *tx = PackageKitBackend::mkGetUpdatesTransaction(pfilters);
    connect(tx, &PackageKit::Transaction::package, this, &XGetUpdatesTransaction::onTransactionPackage);
    return tx;
}
