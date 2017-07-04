#ifndef XChainTransaction_hpp
#define XChainTransaction_hpp

#include <QObject>
#include <QVector>

#include "XTransaction.hpp"

class XChainTransaction : public XTransaction
{
    Q_OBJECT
public:
    XChainTransaction(QObject *parent = nullptr);
    ~XChainTransaction();

    XChainTransaction &operator<<(XTransaction *transaction);

    QVector<XTransaction*> getTransactions() const;

    XTransaction *currentTransaction() const;

    XTransaction *first() const;
    XTransaction *last() const;

signals:
    void transactionStarted(XTransaction *transaction);
    void transactionFinished(XTransaction *transaction);
    void transactionFailed(XTransaction *transaction, const QVariantMap &details);

protected:
    void startEvent() override;

protected slots:
    void onTransactionFinished(XTransaction *transaction);
    void runTransactionAtIndex(int index);

private:
    class Private;
    Private *d;

};

#endif // XChainTransaction_hpp
