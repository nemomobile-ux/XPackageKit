#include <QCoreApplication>
#include <QCommandLineParser>
#include <QCommandLineOption>
#include <QDebug>
#include <QTimer>

#include <QMap>

#include "XPackageKit/XTransaction.hpp"
#include "XPackageKit/XTransactionManager.hpp"

enum class Command {
    InstallPackage,
    RemovePackage,
    AddRepo,
    EnableRepo,
    DisableRepo,
    RemoveRepo,
    UpdateRepo,
    UpdatePackage,
};

QMap<Command,QString> s_descriptionMap = {
    { Command::InstallPackage, "Install package" },
    { Command::RemovePackage, "Remove package" },
    { Command::AddRepo, "Add repo"},
    { Command::EnableRepo, "Enable repo" },
    { Command::DisableRepo, "Disable repo" },
    { Command::RemoveRepo, "Remove repo" },
    { Command::UpdateRepo, "Update repo" },
    { Command::UpdatePackage, "Update package" },
};

QMap<QString,Command> s_commandMap = {
    { "in", Command::InstallPackage },
    { "rm", Command::RemovePackage },
    { "ar", Command::AddRepo },
    { "er", Command::EnableRepo },
    { "dr", Command::DisableRepo },
    { "rr", Command::RemoveRepo },
    { "ur", Command::UpdateRepo },
    { "ref", Command::UpdateRepo },
    { "up", Command::UpdatePackage },
};

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    app.setApplicationName(QStringLiteral("XPackage manager"));
    QCommandLineParser parser;
    parser.addHelpOption();

    parser.addPositionalArgument("command", "The command to execute.");

    for (const QString &key : s_commandMap.keys()) {
        Command c = s_commandMap.value(key);
        parser.addPositionalArgument(key, s_descriptionMap.value(c));
    }

    // Call parse() to find out the positional arguments.
    parser.parse(app.arguments());

    const QStringList initialArgs = parser.positionalArguments();
    const QString commandStr = initialArgs.isEmpty() ? QString() : initialArgs.first();

    if (!s_commandMap.contains(commandStr)) {
        parser.showHelp(1); // exit 1
    }
    Command c = s_commandMap.value(commandStr);

    parser.clearPositionalArguments();

    QCommandLineOption allowDepsOption({ QStringLiteral("d"), QStringLiteral("allow-deps") },
                                 QStringLiteral("allow the package manager to remove other packages which depend on the packages to be removed"));

    QCommandLineOption autoRemoveOption({ QStringLiteral("a"), QStringLiteral("auto-remove") },
                                 QStringLiteral("tells the package manager to remove all the package which won't be needed anymore after the packages are uninstalled"));

    QCommandLineOption downloadOnly({ QStringLiteral("s"), QStringLiteral("download-only") },
                                 QStringLiteral("only download and store rpm packages in cache, do not install"));

    switch (c) {
    case Command::InstallPackage:
        parser.addPositionalArgument("package", "Package name");
        parser.addOption(downloadOnly);
        break;
    case Command::RemovePackage:
        parser.addPositionalArgument("package", "Package name");
        parser.addOption(allowDepsOption);
        parser.addOption(autoRemoveOption);
        break;
    case Command::AddRepo:
    case Command::EnableRepo:
    case Command::DisableRepo:
    case Command::RemoveRepo:
    case Command::UpdateRepo:
        parser.addPositionalArgument("repo", "Repository name");
    case Command::UpdatePackage:
        parser.addPositionalArgument("package", "Package name");
        parser.addOption(downloadOnly);
        break;
    default:
        break;
    }
    parser.process(app);

    const QStringList names = initialArgs.mid(1);

    qDebug() << "n:" << initialArgs;
    qDebug() << "n:" << names;
    if (names.isEmpty()) {
        return 2;
    }

    XTransactionManager::setDefaultParent(&app);

    XTransaction *tx = nullptr;
    switch (c) {
    case Command::InstallPackage:
        if (names.count() < 1) {
            parser.showHelp(2);
        }
        tx = XTransactionManager::installPackages(names, parser.isSet(downloadOnly) ? XTransactionNamespace::TransactionFlagOnlyDownload
                                                                                    : XTransactionNamespace::TransactionFlagNone);
        break;
    case Command::RemovePackage:
        if (names.count() < 1) {
            parser.showHelp(2);
        }
        tx = XTransactionManager::removePackages({names.first()});
        if (parser.isSet(allowDepsOption)) {
            tx->setRequestDetail("allowDeps", true);
        }
        if (parser.isSet(autoRemoveOption)) {
            tx->setRequestDetail("autoRemove", true);
        }
        break;
    case Command::AddRepo:
    {
        if (names.count() < 2) {
            parser.showHelp(2);
        }
        const QString url = names.at(1);
        tx = XTransactionManager::addRepository(names.first(), {{"url", url}});
    }
        break;
    case Command::EnableRepo:
        tx = XTransactionManager::setRepositoryEnabled(names.first(), true);
        break;
    case Command::DisableRepo:
        tx = XTransactionManager::setRepositoryEnabled(names.first(), false);
        break;
    case Command::RemoveRepo:
        tx = XTransactionManager::removeRepository(names.first());
        break;
    case Command::UpdateRepo:
        tx = XTransactionManager::refreshRepository(names.first());
        break;
    case Command::UpdatePackage:
        if (names.count() < 1) {
            parser.showHelp(2);
        }
        tx = XTransactionManager::updatePackages(names, parser.isSet(downloadOnly) ? XTransactionNamespace::TransactionFlagOnlyDownload
                                                                                   : XTransactionNamespace::TransactionFlagNone);
        break;
    default:
        break;
    }

    if (!tx) {
        parser.showHelp(3);
    }

    QObject::connect(tx, &XTransaction::finished, [](XTransaction *transaction) {
        if (transaction->isSucceeded()) {
            QCoreApplication::exit();
        } else {
            QCoreApplication::exit(5);
        }
    });
    QObject::connect(tx, &XTransaction::failed, [](XTransaction *transaction, const QVariantHash &details) {
        qWarning() << "Transaction failed:" << transaction << details;
    });

    QTimer::singleShot(0, tx, &XTransaction::start);

    return app.exec();
}
