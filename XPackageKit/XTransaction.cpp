#include "XTransaction.hpp"

#include <QDebug>

XTransaction::XTransaction(RequestType type, QObject *parent) :
    QObject(parent),
    m_running(false),
    m_finished(false),
    m_succeeded(true),
    m_type(type)
{
}

void XTransaction::setRequestDetails(const QVariantHash &details)
{
    m_requestDetails = details;
}

void XTransaction::setRequestDetail(const QString &key, const QVariant &value)
{
    m_requestDetails.insert(key, value);
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

void XTransaction::restart()
{
    qDebug() << Q_FUNC_INFO;
    if (m_running) {
        qWarning() << "Unable to restart: transaction is already running (type:" << m_type << "details:" << m_requestDetails << ")";
        return;
    }

    if (m_finished) {
        qDebug() << "Actually restart transaction";
        m_finished = false;
    }

    start();
}

void XTransaction::start()
{
    qDebug() << Q_FUNC_INFO;
    if (m_running) {
        qWarning() << "Unable to start: transaction is already running (type:" << m_type << "details:" << m_requestDetails << ")";
        return;
    }

    if (m_finished) {
        qWarning() << "Unable to start: transaction is already finished (type:" << m_type << "details:" << m_requestDetails << ")";
        return;
    }
    m_running = true;
    emit started(this);
    startEvent();
}

void XTransaction::setFinished()
{
    qDebug() << Q_FUNC_INFO;
    m_finished = true;
    m_running = false;
    if (m_succeeded) {
        emit succeeded(this);
    }
    emit finished(this);
}

void XTransaction::setFinishedWithError(const QVariantHash &details)
{
    qDebug() << Q_FUNC_INFO << details;
    m_succeeded = false;
    emit failed(this, details);
    setFinished();
}

void XTransaction::setDelayedFinishedWithError(const QVariantHash &details)
{
    qDebug() << Q_FUNC_INFO << details;
    QMetaObject::invokeMethod(this, "setFinishedWithError", Qt::QueuedConnection, Q_ARG(QVariantHash, details)); // Invoke after return
}

void XTransaction::onPreviousTransactionFailed(XTransaction *transaction, const QVariantHash &details)
{
    setFinishedWithError({{QStringLiteral("text"), tr("Previous transaction failed")}});
}
