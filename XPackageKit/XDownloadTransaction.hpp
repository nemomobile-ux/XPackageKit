#ifndef XDOWNLOADTRANSACTION_HPP
#define XDOWNLOADTRANSACTION_HPP

#include "XPackageKitTransaction.hpp"
#include <Transaction>

class XDownloadTransaction : public XPackageKitTransaction
{
    Q_OBJECT
public:
    XDownloadTransaction(QObject *parent = nullptr);

private slots:
    void onTransactionPackage(PackageKit::Transaction::Info info, const QString &packageID, const QString &summary);

protected:
    PackageKit::Transaction *createTransaction() override;
};

#endif // XDOWNLOADTRANSACTION_HPP
