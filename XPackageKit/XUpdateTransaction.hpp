#ifndef XUPDATETRANSACTION_HPP
#define XUPDATETRANSACTION_HPP

#include "XPackageKitTransaction.hpp"

class XUpdateTransaction : public XPackageKitTransaction
{
    Q_OBJECT
public:
    XUpdateTransaction(const QString &newObjectName, QObject *parent = nullptr);

private slots:
    void onTransactionPackage(PackageKit::Transaction::Info info, const QString &packageID, const QString &summary);

protected:
    PackageKit::Transaction *createTransaction() override;
};

#endif // XUPDATETRANSACTION_HPP
