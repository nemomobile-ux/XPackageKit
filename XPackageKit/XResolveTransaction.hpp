#ifndef XRESOLVETRANSACTION_HPP
#define XRESOLVETRANSACTION_HPP

#include "XPackageKitTransaction.hpp"
#include <Transaction>

class XResolveTransaction : public XPackageKitTransaction
{
    Q_OBJECT
public:
    XResolveTransaction(QObject *parent = nullptr);

protected:
    PackageKit::Transaction *createTransaction() override;

private slots:
    void onTransactionPackage(PackageKit::Transaction::Info info, const QString &packageID, const QString &summary);
};

#endif // XRESOLVETRANSACTION_HPP
