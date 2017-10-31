#include "XResolveTransaction.hpp"
#include "PackageKitBackend.hpp"

#include <QDebug>

XResolveTransaction::XResolveTransaction(QObject *parent)
    : XPackageKitTransaction(parent)
{
}

PackageKit::Transaction *XResolveTransaction::createTransaction()
{
    const QStringList packageNames = requestDetails().value(QStringLiteral("packageNames")).toStringList();
    const XTransactionNamespace::Filters xfilters = requestDetails().value(QStringLiteral("filters")).value<XTransactionNamespace::Filters>();
    PackageKit::Transaction *tx = PackageKitBackend::mkResolveTransaction(packageNames, toPackageFilters(xfilters));
    connect(tx, &PackageKit::Transaction::package, this, &XResolveTransaction::onTransactionPackage);
    return tx;
}

void XResolveTransaction::onTransactionPackage(PackageKit::Transaction::Info info, const QString &packageID, const QString &summary)
{
    addResult({
                  {QStringLiteral("info"), QVariant::fromValue(info)},
                  {QStringLiteral("packageID"), packageID},
                  {QStringLiteral("summary"), summary},
              });
}
