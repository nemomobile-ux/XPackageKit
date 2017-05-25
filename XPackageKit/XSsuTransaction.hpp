#ifndef XSsuTransaction_hpp
#define XSsuTransaction_hpp

#include "XTransaction.hpp"

QT_FORWARD_DECLARE_CLASS(QDBusPendingCallWatcher)

class XSsuTransaction : public XTransaction
{
    Q_OBJECT
public:
    explicit XSsuTransaction(RequestType type, QObject *parent = nullptr);
    QString repoName() const;

protected slots:
    void onSsuCallReply(QDBusPendingCallWatcher *watcher);

protected:
    enum class SsuRepoAction {
        // Keep in sync with Ssud::Actions
        Remove  = 0,
        Add     = 1,
        Disable = 2,
        Enable  = 3,
        Invalid
    };

    void startEvent() override;

    void addRepo();
    void removeRepo();
    void setRepoEnabled();
    void modifyRepo(SsuRepoAction action);

    void callSsuMethod(const QString &method, const QVariantList &arguments);

};

#endif // XSsuTransaction_hpp

