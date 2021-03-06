#ifndef XGETDEPENDSTRANSACTION_HPP
#define XGETDEPENDSTRANSACTION_HPP

#include "XPackageKitTransaction.hpp"

class XGetDependsTransaction : public XPackageKitTransaction
{
    Q_OBJECT
public:
    XGetDependsTransaction(const QString &newObjectName, QObject *parent = nullptr);

private slots:
    void onTransactionPackage(PackageKit::Transaction::Info info, const QString &packageID, const QString &summary);

protected:
    PackageKit::Transaction *createTransaction() override;
};

#endif // XGETDEPENDSTRANSACTION_HPP
