#include "XSsuTransaction.hpp"

#include <QDBusConnection>
#include <QDBusPendingCall>
#include <QDBusPendingCallWatcher>
#include <QDBusPendingReply>
#include <QDBusMessage>

#include <QDebug>

XSsuTransaction::XSsuTransaction(QObject *parent)
: XTransaction(parent)
{
    setStatus(XTransactionNamespace::RepoStatusIdle);
}

QString XSsuTransaction::repoName() const
{
    return requestDetails().value(QStringLiteral("repoName")).toString();
}

void XSsuTransaction::startEvent()
{
    xDebug() << requestDetails();

    switch (action()) {
    case SsuRepoAction::Add:
        addRepo();
        break;
    case SsuRepoAction::Remove:
    case SsuRepoAction::Enable:
    case SsuRepoAction::Disable:
        modifyRepo(action());
        break;
    default:
        break;
    }
}

void XSsuTransaction::addRepo()
{
    const QString url = requestDetails().value(QStringLiteral("url")).toString();
    if (repoName().isEmpty() || url.isEmpty()) {
        setDelayedFinishedWithError(QVariantHash({{QStringLiteral("text"), tr("Invalid arguments (we need repo name and repo url)")}}));
        return;
    }

    callSsuMethod(QStringLiteral("addRepo"), { repoName(), url });
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

    setStatus(XTransactionNamespace::RepoStatusWorking);
    setProgress(50);
}

XSsuTransaction::SsuRepoAction XSsuTransaction::action()
{
    return requestDetails().value(QStringLiteral("action")).value<SsuRepoAction>();
}

void XSsuTransaction::onSsuCallReply(QDBusPendingCallWatcher *watcher)
{
    setProgress(100);
    setStatus(XTransactionNamespace::RepoStatusCompleted);

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
