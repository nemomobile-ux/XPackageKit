#include "XSsuTransaction.hpp"

#include <QDBusConnection>
#include <QDBusPendingCall>
#include <QDBusPendingCallWatcher>
#include <QDBusPendingReply>
#include <QDBusMessage>

#include <QDebug>


XSsuTransaction::XSsuTransaction(XTransaction::RequestType type, const QString &name, QObject *parent) :
    XTransaction(type, parent),
    m_repoName(name)
{
}

void XSsuTransaction::startEvent()
{
    qDebug() << Q_FUNC_INFO << m_repoName << m_requestDetails;

    switch (m_type) {
    case AddRepoRequest:
        addRepo();
        break;
    case RemoveRepoRequest:
        removeRepo();
        break;
    case SetRepoEnabledRequest:
        setRepoEnabled();
        break;
    default:
        break;
    }
}

void XSsuTransaction::addRepo()
{
    const QString url = m_requestDetails.value(QStringLiteral("url")).toString();
    if (m_repoName.isEmpty() || url.isEmpty()) {
        setDelayedFinishedWithError(QVariantMap({{"text", "Invalid arguments (we need repo name and repo url)"}}));
        return;
    }

    callSsuMethod(QStringLiteral("addRepo"), { m_repoName, url });
}

void XSsuTransaction::removeRepo()
{
    if (m_repoName.isEmpty()) {
        setDelayedFinishedWithError(QVariantMap({{"text", "Repo name is not set"}}));
        return;
    }
    modifyRepo(SsuRepoAction::Remove);
}

void XSsuTransaction::setRepoEnabled()
{
    const QVariant enVariant = m_requestDetails.value(QStringLiteral("enable"));
    if (!enVariant.canConvert<bool>()) {
        setDelayedFinishedWithError(QVariantMap({{"text", "Repo 'enable' bool argument is not set"}}));
        return;
    }
    const bool enable = enVariant.toBool();
    if (enable) {
        modifyRepo(SsuRepoAction::Enable);
    } else {
        modifyRepo(SsuRepoAction::Disable);
    }
}

void XSsuTransaction::modifyRepo(XSsuTransaction::SsuRepoAction action)
{
    const int actionInt = static_cast<int>(action);
    callSsuMethod(QStringLiteral("modifyRepo"), { actionInt, m_repoName });
}

void XSsuTransaction::callSsuMethod(const QString &method, const QVariantList &arguments)
{
    QDBusMessage ssuAddRepoCall = QDBusMessage::createMethodCall(
                "org.nemo.ssu",
                "/org/nemo/ssu",
                "org.nemo.ssu",
                method);
    ssuAddRepoCall.setArguments(arguments);
    QDBusPendingCall reply = QDBusConnection::systemBus().asyncCall(ssuAddRepoCall);
    QDBusPendingCallWatcher *watcher = new QDBusPendingCallWatcher(reply, this);
    QObject::connect(watcher, &QDBusPendingCallWatcher::finished, this, &XSsuTransaction::onSsuCallReply);
}

void XSsuTransaction::onSsuCallReply(QDBusPendingCallWatcher *watcher)
{
    watcher->deleteLater();
    if (watcher->isError()) {
        setFinishedWithError({
                                 {"dbusErrorName", watcher->error().name() },
                                 {"dbusErrorMessage", watcher->error().message() },
                                 {"backend_details", tr("SSU call failed")}
                             });
        return;
    }

    setFinished();
}
