#include "XChainTransaction.hpp"

class XChainTransaction::Private
{
public:
    Private() :
        currentTransaction(nullptr),
        currentTransactionIndex(0)
    {
    }

    QVector<XTransaction*> transactions;
    XTransaction *currentTransaction;
    int currentTransactionIndex;
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
    d->transactions.append(transaction);
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

    runTransactionAtIndex(0);
}

void XChainTransaction::onTransactionFinished(XTransaction *transaction)
{
    disconnect(d->currentTransaction, &XTransaction::started, this, &XChainTransaction::transactionStarted);
    disconnect(d->currentTransaction, &XTransaction::failed, this, &XChainTransaction::transactionFailed);
    disconnect(d->currentTransaction, &XTransaction::finished, this, &XChainTransaction::onTransactionFinished);

    emit transactionFinished(transaction);

    const int nextTransactionIndex = d->currentTransactionIndex + 1;
    if (d->transactions.count() > nextTransactionIndex) {
        runTransactionAtIndex(nextTransactionIndex);
    }
}

void XChainTransaction::runTransactionAtIndex(int index)
{
    d->currentTransactionIndex = index;
    d->currentTransaction = d->transactions.at(index);
    connect(d->currentTransaction, &XTransaction::started, this, &XChainTransaction::transactionStarted);
    connect(d->currentTransaction, &XTransaction::failed, this, &XChainTransaction::transactionFailed);
    connect(d->currentTransaction, &XTransaction::finished, this, &XChainTransaction::onTransactionFinished);
    d->currentTransaction->start();
}
