#ifndef XTransaction_hpp
#define XTransaction_hpp

#include <QObject>
#include <QVector>
#include <QVariantHash>

enum PackageStatus {
    PackageInstalled,
    PackageUpdated,
    PackageAvailable,
    PackageNotAvailable
};

class XTransaction : public QObject
{
    Q_OBJECT
    Q_PROPERTY(RequestType type READ type CONSTANT)
public:
    enum RequestType {
        ChainRequest,
        InstallRequest,
        RemoveRequest,
        UpdateRequest,
        StatusRequest,
        AddRepoRequest,
        RemoveRepoRequest,
        SetRepoEnabledRequest,
        RefreshRepoRequest
    };
#if QT_VERSION >= QT_VERSION_CHECK(5, 5, 0)
    Q_ENUM(RequestType);
#else
    Q_ENUMS(RequestType)
#endif

    explicit XTransaction(RequestType type, QObject *parent = nullptr);

    bool isRunning() { return m_running; }
    bool isFinished() { return m_finished; }
    bool isSucceeded() { return m_succeeded; }

    QVariantHash errorDetails() const { return m_errorDetails; }

    QVector<QVariantHash> results() const { return m_results; }
    QVariantHash requestDetails() const { return m_requestDetails; }
    void setRequestDetails(const QVariantHash &details);
    void setRequestDetail(const QString &key, const QVariant &value);

    RequestType type() const { return m_type; }

    bool runAfter(XTransaction *transaction);

signals:
    void started(XTransaction *transaction);
    void finished(XTransaction *transaction);
    void failed(XTransaction *transaction, const QVariantHash &details);
    void succeeded(XTransaction *transaction);

public slots:
    void restart();
    void start();

protected slots:
    void setFinished();
    void setFinishedWithError(const QVariantHash &details);
    void setDelayedFinishedWithError(const QVariantHash &details);
    void onPreviousTransactionFailed(XTransaction *transaction, const QVariantHash &details);
    void addResult(const QVariantHash &result);

protected:
    virtual void startEvent() = 0;

    bool m_running;
    bool m_finished;
    bool m_succeeded;

    RequestType m_type;
    QVector<QVariantHash> m_results;
    QVariantHash m_requestDetails;
    QVariantHash m_errorDetails;

};

#endif // XTransaction_hpp
