#ifndef XGETPACKAGESTRANSACTION_HPP
#define XGETPACKAGESTRANSACTION_HPP

#include "XPackageKitTransaction.hpp"

class XGetPackagesTransaction : public XPackageKitTransaction
{
    Q_OBJECT
public:
    explicit XGetPackagesTransaction(const QString &newObjectName, QObject *parent = nullptr);

private slots:
    void onTransactionPackage(PackageKit::Transaction::Info info, const QString &packageID, const QString &summary);

protected:
    PackageKit::Transaction *createTransaction() override;
};

#endif // XGETPACKAGESTRANSACTION_HPP
