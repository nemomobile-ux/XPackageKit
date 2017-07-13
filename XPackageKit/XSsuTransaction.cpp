#include "XSsuTransaction.hpp"

#include <QDBusConnection>
#include <QDBusPendingCall>
#include <QDBusPendingCallWatcher>
#include <QDBusPendingReply>
#include <QDBusMessage>

#include <QDebug>

XSsuTransaction::XSsuTransaction(XTransaction::RequestType type, QObject *parent) :
    XTransaction(type, parent)
{
}

QString XSsuTransaction::repoName() const
{
    return requestDetails().value(QStringLiteral("repoName")).toString();
}

void XSsuTransaction::startEvent()
{
    qDebug() << Q_FUNC_INFO << m_requestDetails;

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
    if (repoName().isEmpty() || url.isEmpty()) {
        setDelayedFinishedWithError(QVariantMap({{QStringLiteral("text"), tr("Invalid arguments (we need repo name and repo url)")}}));
        return;
    }

    callSsuMethod(QStringLiteral("addRepo"), { repoName(), url });
}

void XSsuTransaction::removeRepo()
{
    if (repoName().isEmpty()) {
        setDelayedFinishedWithError(QVariantMap({{QStringLiteral("text"), tr("Repo name is not set")}}));
        return;
    }
    modifyRepo(SsuRepoAction::Remove);
}

void XSsuTransaction::setRepoEnabled()
{
    const QVariant enVariant = m_requestDetails.value(QStringLiteral("enable"));
    if (!enVariant.canConvert<bool>()) {
        setDelayedFinishedWithError(QVariantMap({{QStringLiteral("text"), tr("Repo 'enable' bool argument is not set")}}));
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
    callSsuMethod(QStringLiteral("modifyRepo"), { actionInt, repoName() });
}

void XSsuTransaction::callSsuMethod(const QString &method, const QVariantList &arguments)
{
    QDBusMessage ssuAddRepoCall = QDBusMessage::createMethodCall(
                QStringLiteral("org.nemo.ssu"),
                QStringLiteral("/org/nemo/ssu"),
                QStringLiteral("org.nemo.ssu"),
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
                                 {QStringLiteral("dbusErrorName"), watcher->error().name() },
                                 {QStringLiteral("dbusErrorMessage"), watcher->error().message() },
                                 {QStringLiteral("backend_details"), tr("SSU call failed")}
                             });
        return;
    }

    setFinished();
}
