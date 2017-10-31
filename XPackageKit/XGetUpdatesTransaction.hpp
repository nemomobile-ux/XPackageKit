#ifndef XGETUPDATESTRANSACTION_HPP
#define XGETUPDATESTRANSACTION_HPP

#include "XPackageKitTransaction.hpp"

class XGetUpdatesTransaction : public XPackageKitTransaction
{
    Q_OBJECT
public:
    XGetUpdatesTransaction(QObject *parent = nullptr);

private slots:
    void onTransactionPackage(PackageKit::Transaction::Info info, const QString &packageID, const QString &summary);

protected:
    PackageKit::Transaction *createTransaction() override;

};

#endif // XGETUPDATESTRANSACTION_HPP
