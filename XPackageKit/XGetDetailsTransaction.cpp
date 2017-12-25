#include "XGetDetailsTransaction.hpp"
#include "PackageKitBackend.hpp"

XGetDetailsTransaction::XGetDetailsTransaction(const QString &newObjectName, QObject *parent)
    : XPackageKitTransaction(newObjectName, parent)
{

}

void XGetDetailsTransaction::onTransactionDetails(const QString &packageID,
                                                  const QString &license,
                                                  PackageKit::Transaction::Group group,
                                                  const QString &detail,
                                                  const QString &url,
                                                  qulonglong size)
{
    addResult({
                  {QStringLiteral("packageID"), packageID},
                  {QStringLiteral("license"), license},
                  {QStringLiteral("group"), QVariant::fromValue(group)},
                  {QStringLiteral("detail"), detail},
                  {QStringLiteral("url"), url},
                  {QStringLiteral("size"), size},
              });
}

PackageKit::Transaction *XGetDetailsTransaction::createTransaction()
{
    const QStringList packageIds = requestDetails().value(QStringLiteral("packageIds")).toStringList();
    PackageKit::Transaction *tx = PackageKitBackend::mkGetDetailsTransaction(packageIds);
    connect(tx, &PackageKit::Transaction::details, this, &XGetDetailsTransaction::onTransactionDetails);
    return tx;
}
