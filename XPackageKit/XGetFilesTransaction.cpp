#include "XGetFilesTransaction.hpp"
#include "PackageKitBackend.hpp"

XGetFilesTransaction::XGetFilesTransaction(QObject *parent)
    : XPackageKitTransaction(parent)
{

}

void XGetFilesTransaction::onTransactionFiles(const QString &packageID, const QStringList &filenames)
{
    addResult({
                  {QStringLiteral("packageID"), packageID},
                  {QStringLiteral("filenames"), filenames},
              });
}

PackageKit::Transaction *XGetFilesTransaction::createTransaction()
{
    const QStringList packageIds = requestDetails().value(QStringLiteral("packageIds")).toStringList();
    PackageKit::Transaction *tx = PackageKitBackend::mkGetFilesTransaction(packageIds);
    connect(tx, &PackageKit::Transaction::files, this, &XGetFilesTransaction::onTransactionFiles);
    return tx;
}
