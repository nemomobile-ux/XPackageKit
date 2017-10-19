#ifndef XPACKAGEKITTRANSACTION_HPP
#define XPACKAGEKITTRANSACTION_HPP

#include "XTransactionManager.hpp"
#include <Transaction>

class XPackageKitTransaction : public XTransaction
{
    Q_OBJECT
public:
    XPackageKitTransaction(QObject *parent = nullptr);

    struct ErrorStruct {
        PackageKit::Transaction::Error error;
        QString details;
    };

    static PackageKit::Transaction::Filters toPackageFilters(XTransactionNamespace::Filters txFilters);
    static PackageKit::Transaction::TransactionFlags toPackageFlags(XTransactionNamespace::TransactionFlags txFlags);

    static PackageKit::Transaction::Status packageStatus(XTransactionNamespace::TransactionStatus txStatus);
    static XTransactionNamespace::TransactionStatus transactionStatus(PackageKit::Transaction::Status pkStatus);

    static QString packageName(const QString &pkgId);
    static QString packageVersion(const QString &pkgId);
    static QString packageArch(const QString &pkgId);
    static QString packageData(const QString &pkgId);

private slots:
    virtual void onTransactionErrorCode(PackageKit::Transaction::Error error, const QString &details);
    virtual void onTransactionFinished(PackageKit::Transaction::Exit exitStatus, uint runtime);
    virtual void onTransactionMessage(PackageKit::Transaction::Message type, const QString &message);
    virtual void onTransactionItemProgress(const QString &itemID, PackageKit::Transaction::Status pkStatus, uint percentage);

    virtual void onTransactionStatusChanged();

protected:
    void startEvent() override;
    virtual PackageKit::Transaction *createTransaction() = 0;

private:
    QVector<ErrorStruct> m_errors;
};

#endif // XPACKAGEKITTRANSACTION_HPP
