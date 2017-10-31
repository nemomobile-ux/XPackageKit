#include "XProcessTransaction.hpp"

XProcessTransaction::XProcessTransaction(QObject *parent)
    : XTransaction(parent)
    , m_process(new QProcess(this))
{
    connect(m_process, (void (QProcess::*)(int))&QProcess::finished, this, &XProcessTransaction::onProcessFinished);
    connect(m_process, &QProcess::stateChanged, this, &XProcessTransaction::onProcessStateChanged);

    setStatus(XTransactionNamespace::ProcessStatusNotRunning);
}

QString XProcessTransaction::program() const
{
    return requestDetails().value(QStringLiteral("program")).toString();
}

QStringList XProcessTransaction::arguments() const
{
    return requestDetails().value(QStringLiteral("arguments")).toStringList();
}

void XProcessTransaction::startEvent()
{
    m_process->start(program(), arguments());
    setProgress(50);
}

void XProcessTransaction::onProcessFinished(int exitCode)
{
    m_process->deleteLater();
    disconnect(m_process, (void (QProcess::*)(int))&QProcess::finished, this, &XProcessTransaction::onProcessFinished);
    disconnect(m_process, &QProcess::stateChanged, this, &XProcessTransaction::onProcessStateChanged);
    setProgress(100);

    const QByteArray output = m_process->readAll();

    if (exitCode != 0) {
        setFinishedWithError({
                                 {QStringLiteral("processExitCode"), exitCode },
                                 {QStringLiteral("processOutput"), output },
                                 {QStringLiteral("processError"), m_process->error() }
                             });
        return;
    };

    addResult({
                  {QStringLiteral("processExitCode"), exitCode},
                  {QStringLiteral("processOutput"), output},
              });

    setFinished();
}

void XProcessTransaction::onProcessStateChanged(QProcess::ProcessState state)
{
    XTransactionNamespace::TransactionStatus transactionStatus = XTransactionNamespace::PackageStatusUnknown;

    switch (state) {
    case QProcess::NotRunning:
        if (status() == XTransactionNamespace::ProcessStatusRunning) {
            transactionStatus = XTransactionNamespace::ProcessStatusFinished;
        } else {
            transactionStatus = XTransactionNamespace::ProcessStatusNotRunning;
        }
        break;
    case QProcess::Starting:
        transactionStatus = XTransactionNamespace::ProcessStatusStarting;
        break;
    case QProcess::Running:
        transactionStatus = XTransactionNamespace::ProcessStatusRunning;
        break;
    }

    setStatus(transactionStatus);
}
