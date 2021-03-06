#ifndef XREMOVETRANSACTION_HPP
#define XREMOVETRANSACTION_HPP

#include "XPackageKitTransaction.hpp"

class XRemoveTransaction : public XPackageKitTransaction
{
    Q_OBJECT
public:
    XRemoveTransaction(const QString &newObjectName, QObject *parent = nullptr);

private slots:
    void onTransactionPackage(PackageKit::Transaction::Info info, const QString &packageID, const QString &summary);

protected:
    PackageKit::Transaction *createTransaction() override;
};

#endif // XREMOVETRANSACTION_HPP
