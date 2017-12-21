#ifndef XGETFILESTRANSACTION_HPP
#define XGETFILESTRANSACTION_HPP

#include "XPackageKitTransaction.hpp"

class XGetFilesTransaction : public XPackageKitTransaction
{
    Q_OBJECT
public:
    XGetFilesTransaction(QObject *parent = nullptr);

private slots:
    void onTransactionFiles(const QString &packageID, const QStringList &filenames);

protected:
    PackageKit::Transaction *createTransaction() override;
};

#endif // XGETFILESTRANSACTION_HPP
