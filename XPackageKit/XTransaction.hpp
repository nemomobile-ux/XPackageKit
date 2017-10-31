#ifndef XTransaction_hpp
#define XTransaction_hpp

#include <QObject>
#include <QVector>
#include <QVariantHash>
#include <QTextStream>

#include "XTransactionNamespace.hpp"

#define xLogItem QStringLiteral("[%1]").arg(objectName())

#define xDebug() QMessageLogger(QT_MESSAGELOG_FILE, QT_MESSAGELOG_LINE, QT_MESSAGELOG_FUNC).debug() << xLogItem
#define xInfo() QMessageLogger(QT_MESSAGELOG_FILE, QT_MESSAGELOG_LINE, QT_MESSAGELOG_FUNC).info() << xLogItem
#define xWarning() QMessageLogger(QT_MESSAGELOG_FILE, QT_MESSAGELOG_LINE, QT_MESSAGELOG_FUNC).warning() << xLogItem
#define xCritical() QMessageLogger(QT_MESSAGELOG_FILE, QT_MESSAGELOG_LINE, QT_MESSAGELOG_FUNC).critical() << xLogItem
#define xFatal() QMessageLogger(QT_MESSAGELOG_FILE, QT_MESSAGELOG_LINE, QT_MESSAGELOG_FUNC).fatal() << xLogItem

class XTransaction : public QObject
{
    Q_OBJECT
public:
    explicit XTransaction(QObject *parent = nullptr);
    bool isRunning() const;
    bool isFinished() const;
    bool isSucceeded() const;
    bool exitOnFailure() const;
    uint progress() const;
    XTransactionNamespace::TransactionStatus status() const;

    void setExitOnFailure(bool exitOnFailure);

    QVariantHash errorDetails() const;

    QVector<QVariantHash> results() const;
    QVariantHash requestDetails() const;
    void setRequestDetails(const QVariantHash &details);
    void setRequestDetail(const QString &key, const QVariant &value);

    bool runAfter(XTransaction *transaction);

signals:
    void started(XTransaction *transaction);
    void finished(XTransaction *transaction);
    void failed(XTransaction *transaction, const QVariantHash &details);
    void succeeded(XTransaction *transaction);

    void progressChanged(XTransaction *transaction, uint percentage);
    void progressStatus(XTransaction *transaction, const QString &itemName, XTransactionNamespace::TransactionStatus status, uint progress);

    void statusChanged(XTransaction *transaction, XTransactionNamespace::TransactionStatus status);

public slots:
    void restart();
    void start();

protected slots:
    void setFinished();
    void setFinishedWithError(const QVariantHash &details);
    void setDelayedFinishedWithError(const QVariantHash &details);
    void onPreviousTransactionFailed();
    void addResult(const QVariantHash &result);
    void setProgress(uint percentage);
    void setStatus(XTransactionNamespace::TransactionStatus status);

protected:
    virtual void startEvent() = 0;

private:
    bool m_running;
    bool m_finished;
    bool m_succeeded;
    bool m_exitOnFailure;
    uint m_progress;
    XTransactionNamespace::TransactionStatus m_status;

    QVector<QVariantHash> m_results;
    QVariantHash m_requestDetails;
    QVariantHash m_errorDetails;

};

#endif // XTransaction_hpp
