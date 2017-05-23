#ifndef XTransaction_hpp
#define XTransaction_hpp

#include <QObject>
#include <QVector>
#include <QVariantMap>

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

    bool isFinished() { return m_finished; }
    bool isSucceeded() { return m_succeeded; }

    QVariantMap errorDetails() const { return m_errorDetails; }

    QVariantMap requestDetails() const { return m_requestDetails; }
    void setRequestDetails(const QVariantMap &details);

    RequestType type() const { return m_type; }

    bool runAfter(XTransaction *transaction);

signals:
    void started(XTransaction *transaction);
    void stepFinished();
    void packageStatusChanged(const QString &packageName, PackageStatus);

    void finished(XTransaction *transaction);
    void failed(XTransaction *transaction, const QVariantMap &details);
    void succeeded();

public slots:
    void start();

protected slots:
    void setFinished();
    void setFinishedWithError(const QVariantMap &details);
    void setDelayedFinishedWithError(const QVariantMap &details);
    void onPreviousTransactionFailed(XTransaction *transaction, const QVariantMap &details);

protected:
    virtual void startEvent() = 0;

    bool m_finished;
    bool m_succeeded;

    RequestType m_type;
    QVariantMap m_requestDetails;
    QVariantMap m_errorDetails;

};

#endif // XTransaction_hpp
