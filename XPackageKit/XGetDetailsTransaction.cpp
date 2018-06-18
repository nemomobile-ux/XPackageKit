#include "XGetDetailsTransaction.hpp"
#include "PackageKitBackend.hpp"

XGetDetailsTransaction::XGetDetailsTransaction(const QString &newObjectName, QObject *parent)
    : XPackageKitTransaction(newObjectName, parent)
{

}

void XGetDetailsTransaction::onTransactionDetails(const PackageKit::Details &values)
{
    addResult({
                  {QStringLiteral("packageID"), values.packageId()},
                  {QStringLiteral("license"), values.license()},
                  {QStringLiteral("group"), QVariant::fromValue(values.group())},
                  {QStringLiteral("description"), values.description()},
                  {QStringLiteral("summary"), values.summary()},
                  {QStringLiteral("url"), values.url()},
                  {QStringLiteral("size"), values.size()},
              });
}

PackageKit::Transaction *XGetDetailsTransaction::createTransaction()
{
    const QStringList packageIds = requestDetails().value(QStringLiteral("packageIds")).toStringList();
    PackageKit::Transaction *tx = PackageKitBackend::mkGetDetailsTransaction(packageIds);
    connect(tx, &PackageKit::Transaction::details, this, &XGetDetailsTransaction::onTransactionDetails);
    return tx;
}
