#include "XChainTransaction.hpp"

class XChainTransaction::Private
{
public:
    Private() :
        currentTransaction(nullptr)
    {
    }

    QVector<XTransaction*> transactions;
    XTransaction *currentTransaction;
};

XChainTransaction::XChainTransaction(QObject *parent) :
    XTransaction(ChainRequest, parent),
    d(new Private)
{
}

XChainTransaction::~XChainTransaction()
{
    delete d;
}

XChainTransaction &XChainTransaction::operator<<(XTransaction *transaction)
{
    if (!d->transactions.isEmpty()) {
        transaction->runAfter(d->transactions.last());
    }
    d->transactions.append(transaction);
    connect(transaction, &XTransaction::started, this, &XChainTransaction::transactionStarted);
    return *this;
}

QVector<XTransaction *> XChainTransaction::getTransactions() const
{
    return d->transactions;
}

XTransaction *XChainTransaction::currentTransaction() const
{
    return d->currentTransaction;
}

void XChainTransaction::startEvent()
{
    if (d->transactions.isEmpty()) {
        QMetaObject::invokeMethod(this, "finished", Qt::QueuedConnection); // Invoke after return
        return;
    }
    d->transactions.first()->start();
}
