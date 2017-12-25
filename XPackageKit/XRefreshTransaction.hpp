#ifndef XREFRESHTRANSACTION_H
#define XREFRESHTRANSACTION_H

#include "XPackageKitTransaction.hpp"
#include <Transaction>

class XRefreshTransaction : public XPackageKitTransaction
{
    Q_OBJECT
public:
    XRefreshTransaction(const QString &newObjectName, QObject *parent = nullptr);

protected:
    PackageKit::Transaction *createTransaction() override;
};

#endif // XREFRESHTRANSACTION_H
