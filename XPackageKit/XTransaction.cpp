#include "XTransaction.hpp"

#include <QDebug>

XTransaction::XTransaction(RequestType type, QObject *parent) :
    QObject(parent),
    m_finished(false),
    m_succeeded(true),
    m_type(type)
{
}

void XTransaction::setRequestDetails(const QVariantMap &details)
{
    m_requestDetails = details;
}

bool XTransaction::runAfter(XTransaction *transaction)
{
    if (!transaction) {
        return false;
    }
    connect(transaction, &XTransaction::succeeded, this, &XTransaction::start);
    connect(transaction, &XTransaction::failed, this, &XTransaction::onPreviousTransactionFailed);
    return true;
}

void XTransaction::start()
{
    qDebug() << Q_FUNC_INFO;
    emit started(this);
    startEvent();
}

void XTransaction::setFinished()
{
    qDebug() << Q_FUNC_INFO;
    m_finished = true;
    emit finished(this);
}

void XTransaction::setFinishedWithError(const QVariantMap &details)
{
    qDebug() << Q_FUNC_INFO << details;
    m_succeeded = false;
    emit failed(this, details);
    setFinished();
}

void XTransaction::setDelayedFinishedWithError(const QVariantMap &details)
{
    qDebug() << Q_FUNC_INFO << details;
    QMetaObject::invokeMethod(this, "setFinishedWithError", Qt::QueuedConnection, Q_ARG(QVariantMap, details)); // Invoke after return
}

void XTransaction::onPreviousTransactionFailed(XTransaction *transaction, const QVariantMap &details)
{
    setFinishedWithError({{"text", tr("Previous transaction failed")}});
}
