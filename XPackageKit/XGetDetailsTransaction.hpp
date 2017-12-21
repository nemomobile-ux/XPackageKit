#ifndef XGETDETAILSTRANSACTION_HPP
#define XGETDETAILSTRANSACTION_HPP

#include "XPackageKitTransaction.hpp"

class XGetDetailsTransaction : public XPackageKitTransaction
{
    Q_OBJECT
public:
    XGetDetailsTransaction(QObject *parent = nullptr);

private slots:
    void onTransactionDetails(const QString &packageID,
                              const QString &license,
                              PackageKit::Transaction::Group group,
                              const QString &detail,
                              const QString &url,
                              qulonglong size);

protected:
    PackageKit::Transaction *createTransaction() override;
};

#endif // XGETDETAILSTRANSACTION_HPP
