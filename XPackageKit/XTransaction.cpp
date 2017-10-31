#include "XTransaction.hpp"
#include "XTransactionManager.hpp"

#include <QDebug>

/*!
    \class XTransaction
    \brief Base class for transactions
    \inmodule XPackageKit
*/

/*!
    \fn XTransaction::startEvent()

    Starting transaction
*/

/*!
    \fn XTransaction::started(XTransaction *transaction);

    \a transaction
*/

/*!
    \fn XTransaction::finished(XTransaction *transaction);

    \a transaction
*/

/*!
    \fn XTransaction::failed(XTransaction *transaction, const QVariantHash &details);

    \a transaction \a details
*/

/*!
    \fn XTransaction::succeeded(XTransaction *transaction);

    \a transaction
*/

/*!
    \fn XTransaction::progressChanged(XTransaction *transaction, uint percentage);

    \a transaction \a percentage
*/

/*!
    \fn XTransaction::progressStatus(XTransaction *transaction, const QString &itemName, XTransactionNamespace::TransactionStatus status, uint progress);

    \a transaction \a itemName \a status \a progress
*/

/*!
    \fn XTransaction::statusChanged(XTransaction *transaction, XTransactionNamespace::TransactionStatus status);

    \a transaction \a status
*/

/*!
   \a parent
*/

XTransaction::XTransaction(QObject *parent)
    : QObject(parent)
    , m_running(false)
    , m_finished(false)
    , m_succeeded(true)
    , m_exitOnFailure(true)
    , m_progress(0)
    , m_status(XTransactionNamespace::TransactionStatusUnknown)
{
}

/*!
    \fn bool XTransaction::isRunning() const

    Returns \c true if \c false otherwise.
*/

bool XTransaction::isRunning() const
{
    return m_running;
}

/*!
    \fn bool XTransaction::isFinished() const

    Returns \c true if \c false otherwise.
*/

bool XTransaction::isFinished() const
{
    return m_finished;
}

/*!
    \fn bool XTransaction::isSucceeded() const

    Returns \c true if \c false otherwise.
*/

bool XTransaction::isSucceeded() const
{
    return m_succeeded;
}

/*!
    \fn bool XTransaction::exitOnFailure() const

    Returns \c true if \c false otherwise.
*/

bool XTransaction::exitOnFailure() const
{
    return m_exitOnFailure;
}

/*!
    \fn uint XTransaction::progress() const

    Returns something
*/

uint XTransaction::progress() const
{
    return m_progress;
}

/*!
    \fn XTransactionNamespace::TransactionStatus XTransaction::status() const

    Returns something
*/

XTransactionNamespace::TransactionStatus XTransaction::status() const
{
    return m_status;
}

/*!
    \fn void XTransaction::setExitOnFailure(bool exitOnFailure)

    \a exitOnFailure
*/

void XTransaction::setExitOnFailure(bool exitOnFailure)
{
    m_exitOnFailure = exitOnFailure;
}

/*!
    \fn QVariantHash XTransaction::errorDetails() const

    Returns something
*/

QVariantHash XTransaction::errorDetails() const
{
    return m_errorDetails;
}

/*!
    \fn QVector<QVariantHash> XTransaction::results() const

    Returns something
*/

QVector<QVariantHash> XTransaction::results() const
{
    return m_results;
}

/*!
    \fn QVariantHash XTransaction::requestDetails() const

    Returns something
*/

QVariantHash XTransaction::requestDetails() const
{
    return m_requestDetails;
}

/*!
    \fn void XTransaction::setRequestDetails(const QVariantHash &details)

    Replaces transaction requestDetails with \a details.
*/

void XTransaction::setRequestDetails(const QVariantHash &details)
{
    m_requestDetails = details;
}

/*!
    \fn void XTransaction::setRequestDetail(const QString &key, const QVariant &value)

    \a key \a value
*/

void XTransaction::setRequestDetail(const QString &key, const QVariant &value)
{
    m_requestDetails.insert(key, value);
}

/*!
    \fn bool XTransaction::runAfter(XTransaction *transaction)

    \a transaction

    Returns something
*/

bool XTransaction::runAfter(XTransaction *transaction)
{
    if (!transaction) {
        return false;
    }
    connect(transaction, &XTransaction::succeeded, this, &XTransaction::start);
    connect(transaction, &XTransaction::failed, this, [this]() {
        setFinishedWithError({{QStringLiteral("text"), tr("Previous transaction failed")}});
    });
    return true;
}

/*!
    \fn void XTransaction::restart()
*/

void XTransaction::restart()
{
    xDebug() << "###";
    if (m_running) {
        xWarning() << "Unable to restart: transaction is already running (details:" << m_requestDetails << ")";
        return;
    }

    if (m_finished) {
        xDebug() << "Actually restart transaction";
        m_finished = false;
    }

    start();
}

/*!
    \fn void XTransaction::start()
*/

void XTransaction::start()
{
    xDebug() << objectName();
    xDebug() << requestDetails();
    if (requestDetails().contains(QStringLiteral("filters"))) {
        xDebug() << requestDetails().value(QStringLiteral("filters")).value<XTransactionNamespace::Filters>();
    }
    if (requestDetails().contains(QStringLiteral("transactionFlags"))) {
        xDebug() << requestDetails().value(QStringLiteral("transactionFlags")).value<XTransactionNamespace::TransactionFlags>();
    }
    if (m_running) {
        xWarning() << "Unable to start: transaction is already running (details:" << m_requestDetails << ")";
        return;
    }

    if (m_finished) {
        xWarning() << "Unable to start: transaction is already finished (details:" << m_requestDetails << ")";
        return;
    }
    m_running = true;
    emit started(this);
    startEvent();
}

/*!
    \fn void XTransaction::setFinished()
*/

void XTransaction::setFinished()
{
    xDebug() << objectName();
    xDebug() << results();
    m_finished = true;
    m_running = false;
    if (m_succeeded) {
        emit succeeded(this);
    }
    emit finished(this);
}

/*!
    \fn void XTransaction::setFinishedWithError(const QVariantHash &details)

    \a details
*/

void XTransaction::setFinishedWithError(const QVariantHash &details)
{
    xDebug() << details;
    m_succeeded = false;
    m_errorDetails = details;
    emit failed(this, details);
    setFinished();
}

/*!
    \fn void XTransaction::setDelayedFinishedWithError(const QVariantHash &details)

    \a details
*/

void XTransaction::setDelayedFinishedWithError(const QVariantHash &details)
{
    xDebug() << details;
    QMetaObject::invokeMethod(this, "setFinishedWithError", Qt::QueuedConnection, Q_ARG(QVariantHash, details)); // Invoke after return
}

/*!
    \fn void XTransaction::onPreviousTransactionFailed(XTransaction *transaction, const QVariantHash &details)

    \a transaction \a details
*/

void XTransaction::onPreviousTransactionFailed()
{
}

/*!
    \fn void XTransaction::addResult(const QVariantHash &result)

    \a result
*/

void XTransaction::addResult(const QVariantHash &result)
{
    m_results.append(result);
}

/*!
    \fn void XTransaction::setProgress(uint percentage)

    \a percentage
*/

void XTransaction::setProgress(uint percentage)
{
    if (m_progress == percentage || percentage > 100) {
        return;
    }

    xDebug() << percentage;

    m_progress = percentage;
    emit progressChanged(this, percentage);
}

/*!
    \fn void XTransaction::setStatus(XTransactionNamespace::TransactionStatus status)

    \a status
*/

void XTransaction::setStatus(XTransactionNamespace::TransactionStatus status)
{
    if (m_status == status) {
        return;
    }

    xDebug() << status;

    m_status = status;
    emit statusChanged(this, status);
}
