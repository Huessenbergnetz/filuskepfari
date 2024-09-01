/*
 * SPDX-FileCopyrightText: (C) 2024 Matthias Fehring / www.huessenbergnetz.de
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "errorcodes.h"
#include "nextcloudusers.h"

#include <QCommandLineOption>
#include <QCommandLineParser>
#include <QCoreApplication>
#include <QDebug>
#include <QLocale>
#include <QTranslator>

using namespace Qt::Literals::StringLiterals;

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    a.setOrganizationName(u"Huessenbergnetz"_s);
    a.setOrganizationDomain(u"huessenbergnetz.de"_s);
    a.setApplicationName(u"filuskepfari"_s);
    a.setApplicationVersion(QStringLiteral(FSKEP_VERSION));

    qSetMessagePattern(QStringLiteral("%{message}"));

    const QLocale locale;
    {
        qDebug() << "Loading translations from" << FSKEP_TRANS_DIR;
        auto trans = new QTranslator(&a);
        if (Q_LIKELY(trans->load(locale,
                                 QCoreApplication::applicationName(),
                                 u"_"_s,
                                 QStringLiteral(FSKEP_TRANS_DIR)))) {
            if (Q_UNLIKELY(!QCoreApplication::installTranslator(trans))) {
                qWarning() << "Can not install translator for" << locale;
            }
        } else {
            qWarning() << "Can not load translations for" << locale << "from" << FSKEP_TRANS_DIR;
        }
    }

    const QStringList supportedServices{u"nextcloudusers"_s};
    const QStringList supportedMimeTypes{u"text/csv"_s};

    QCommandLineParser parser;

    parser.addHelpOption();

    QCommandLineOption input(QStringList{u"i"_s, u"input"_s},
                             //: CLI option description
                             //% "Path to a file containing the input data."
                             qtTrId("fskep_cli_opt_input_desc"),
                             //: CLI option value name
                             //% "file"
                             qtTrId("fskep_cli_opt_input_val"));
    parser.addOption(input);

    QCommandLineOption password(QStringList{u"p"_s, u"password"_s},
                                //: CLI option description
                                //% "Password for the user to authenticate at the service to perform the required actions. "
                                //% "Can also be an environment variable containing the password."
                                qtTrId("fskep_cli_opt_password_desc"),
                                //: CLI option value name
                                //% "password"
                                qtTrId("fskep_cli_opt_password_val"));
    parser.addOption(password);

    QCommandLineOption remote(QStringList{u"r"_s, u"remote"_s},
                              //: CLI option description
                              //% "Address of the remote service."
                              qtTrId("fskep_cli_opt_remote_desc"),
                              //: CLI option value name
                              //% "URL"
                              qtTrId("fskep_cli_opt_remote_val"));
    parser.addOption(remote);

    QCommandLineOption service(QStringList{u"s"_s, u"service"_s},
                               //: CLI option description
                               //% "Selects the service type. Supported services: %1"
                               qtTrId("fskep_cli_opt_service_desc").arg(locale.createSeparatedList(supportedServices)),
                               //: CLI option value name
                               //% "service"
                               qtTrId("fskep_cli_opt_service_val"));
    parser.addOption(service);

    QCommandLineOption showReqParams(u"show-req-params"_s,
                                     //% "Show required parameters for selected service."
                                     qtTrId("fskep_cli_opt_show_req_params_desc"));
    parser.addOption(showReqParams);

    QCommandLineOption user(QStringList{u"u"_s, u"user"_s},
                            //: CLI option description
                            //% "User name or ID of the user to authenticate at the service to perform the required actions. "
                            //% "Can also be an environment variable containing the username."
                            qtTrId("fskep_cli_opt_user_desc"),
                            //: CLI option value name
                            //% "username"
                            qtTrId("fskep_cli_opt_user_val"));
    parser.addOption(user);

    parser.addVersionOption();

    parser.process(a);

    const QString serviceType = parser.value(service).toLower();

    if (serviceType.isEmpty()) {
        //% "Missing service type. Use the %1 option to specify the service."
        qCritical().noquote() << qtTrId("fskep_err_missing_service").arg('-'_L1 + service.names().constFirst());
        return static_cast<int>(Fskep::ErrCode::ConfErr);
    }

    if (!supportedServices.contains(serviceType)) {
        //% "Invalid service type. Supported services: %1"
        qCritical().noquote() << qtTrId("fskep_err_invalid_service").arg(locale.createSeparatedList(supportedServices));
        return static_cast<int>(Fskep::ErrCode::ConfErr);
    }

    Service *serv = nullptr;

    if (serviceType == "nextcloudusers"_L1) {
        serv = new NextcloudUsers{&parser, &a};
    }

    if (!serv) {
        return static_cast<int>(Fskep::ErrCode::AppErr);
    }

    serv->start();

    return a.exec();
}
