#ifndef XGETDETAILSTRANSACTION_HPP
#define XGETDETAILSTRANSACTION_HPP

#include "XPackageKitTransaction.hpp"

class XGetDetailsTransaction : public XPackageKitTransaction
{
    Q_OBJECT
public:
    XGetDetailsTransaction(const QString &newObjectName, QObject *parent = nullptr);

private slots:
    void onTransactionDetails(const PackageKit::Details &values);

protected:
    PackageKit::Transaction *createTransaction() override;
};

#endif // XGETDETAILSTRANSACTION_HPP
