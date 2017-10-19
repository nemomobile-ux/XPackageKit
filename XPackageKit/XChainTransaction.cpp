#include "XChainTransaction.hpp"
#include <QDebug>
#include <QMetaMethod>

class XChainTransaction::Private
{
public:
    Private() :
        currentTransaction(nullptr),
        currentTransactionIndex(0),
        maxTransactionCount(0)
    {
    }

    QVector<XTransaction*> transactions;
    XTransaction *currentTransaction;
    uint currentTransactionIndex;
    uint maxTransactionCount;
};

/*!
    \class XChainTransaction
    \brief Class combinig transactions into sequence
    \inmodule XPackageKit
*/


/*!
    \fn void XChainTransaction::transactionStarted(XTransaction *transaction);

    \a transaction
*/


/*!
    \fn void XChainTransaction::transactionFinished(XTransaction *transaction);

    \a transaction
*/

XChainTransaction::XChainTransaction(QObject *parent)
    : XTransaction(parent)
    , d(new Private)
{
}

XChainTransaction::~XChainTransaction()
{
    delete d;
}

/*!
    \fn XChainTransaction &XChainTransaction::operator<<(XTransaction *transaction)

    \a transaction

    Returns chain
*/

XChainTransaction &XChainTransaction::operator<<(XTransaction *transaction)
{
    xDebug() << "adding" << transaction->objectName() << "to" << objectName();
    d->transactions.append(transaction);
    return *this;
}

/*!
    \fn QVector<XTransaction *> XChainTransaction::getTransactions() const

    Returns vector
*/

QVector<XTransaction *> XChainTransaction::getTransactions() const
{
    return d->transactions;
}

/*!
    \fn XTransaction *XChainTransaction::currentTransaction() const

    Returns transaction
*/

XTransaction *XChainTransaction::currentTransaction() const
{
    return d->currentTransaction;
}

/*!
    \fn XTransaction *XChainTransaction::first() const

    Returns transaction
*/

XTransaction *XChainTransaction::first() const
{
    if (d->transactions.isEmpty()) {
        return nullptr;
    }
    return d->transactions.first();
}

/*!
    \fn XTransaction *XChainTransaction::last() const

    Returns transaction
*/

XTransaction *XChainTransaction::last() const
{
    if (d->transactions.isEmpty()) {
        return nullptr;
    }
    return d->transactions.last();
}

/*!
    \fn uint XChainTransaction::transactionsMaxCount() const

    Returns max count
*/

uint XChainTransaction::transactionsMaxCount() const
{
    return d->maxTransactionCount;
}

/*!
    \fn void XChainTransaction::setTransactionsMaxCount(uint count)

    \a count
*/

void XChainTransaction::setTransactionsMaxCount(uint count)
{
    d->maxTransactionCount = count;
}

/*!
    \reimp
*/

void XChainTransaction::startEvent()
{
    if (d->transactions.isEmpty()) {
        QMetaMethod::fromSignal(&XTransaction::finished).invoke(this, Qt::QueuedConnection); // Emit after return
        return;
    }

    if (d->maxTransactionCount == 0) {
        d->maxTransactionCount = d->transactions.count();
    }

    runTransactionAtIndex(0);
}

/*!
    \fn void XChainTransaction::onTransactionFinished(XTransaction *transaction)

    \a transaction
*/

void XChainTransaction::onTransactionFinished(XTransaction *transaction)
{
    disconnect(d->currentTransaction, &XTransaction::finished, this, &XChainTransaction::onTransactionFinished);
    disconnect(d->currentTransaction, &XTransaction::progressChanged, this, &XChainTransaction::onTransactionProgressChanged);

    emit transactionFinished(transaction);

    if (!transaction->isSucceeded()) {
        if (transaction->exitOnFailure()) {
            QVariantHash errorDetails = transaction->errorDetails();
            errorDetails[QStringLiteral("chain-error")] = tr("Inner transaction failed");
            setFinishedWithError(errorDetails);
        }
        return;
    }

    const int nextTransactionIndex = d->currentTransactionIndex + 1;
    if (d->transactions.count() > nextTransactionIndex) {
        runTransactionAtIndex(nextTransactionIndex);
    } else {
        setProgress(100);
        setFinished();
    }
}

/*!
    \fn void XChainTransaction::onTransactionProgressChanged(XTransaction *transaction, uint percentage)

    \a transaction \a percentage
*/

void XChainTransaction::onTransactionProgressChanged(XTransaction *transaction, uint percentage)
{
    if (d->currentTransactionIndex < d->maxTransactionCount) {
        uint progress = 100 / d->maxTransactionCount * d->currentTransactionIndex;
        progress += percentage / d->maxTransactionCount;
        setProgress(progress);
    }
}

/*!
    \fn void XChainTransaction::runTransactionAtIndex(uint index)

    \a index
*/

void XChainTransaction::runTransactionAtIndex(uint index)
{
    d->currentTransactionIndex = index;
    d->currentTransaction = d->transactions.at(index);
    xDebug() << "starting" << d->currentTransaction->objectName() << "from chain" << objectName();
    connect(d->currentTransaction, &XTransaction::finished, this, &XChainTransaction::onTransactionFinished);
    connect(d->currentTransaction, &XTransaction::progressChanged, this, &XChainTransaction::onTransactionProgressChanged);
    d->currentTransaction->start();
    emit transactionStarted(d->currentTransaction);

    if (index < d->maxTransactionCount) {
        setProgress(100 / d->maxTransactionCount * index);
    }
}
