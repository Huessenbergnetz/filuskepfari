/*
 * SPDX-FileCopyrightText: (C) 2024 Matthias Fehring / www.huessenbergnetz.de
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "nextcloudusers.h"

#include "emailparameter.h"
#include "stringparameter.h"

#include <QCommandLineParser>
#include <QDebug>

using namespace Qt::Literals::StringLiterals;

NextcloudUsers::NextcloudUsers(QCommandLineParser *parser, QObject *parent)
    : Service{parser, parent}
{
}

void NextcloudUsers::initParameters()
{

}

Service::Requirements NextcloudUsers::requirements() const
{
    Service::Requirements reqs;
    reqs.setFlag(Service::Requirement::RemoteUrl);
    reqs.setFlag(Service::Requirement::Auth);
    return reqs;
}

void NextcloudUsers::processData(const DataPair &data)
{
    qDebug() << "Headers:" << data.first;
    qDebug() << "Data:" << data.second;
}

QList<Parameter*> NextcloudUsers::parameters()
{
    return {
        //% "Account ID used to login (must only contain a-z, A-Z, 0-9, -, _ and @)"
        new StringParameter{u"uid"_s, qtTrId("fskep_ncusers_param_desc_uid"), Parameter::Required::Yes, u"^[a-zA-Z0-9_@\\.-]+$"_s, this},
        //% "Display name for the new user. Can conain any character."
        new StringParameter{u"displayName"_s, qtTrId("fskep_ncusers_param_desc_displayName"), Parameter::Required::No, {}, this},
        //% "When set, users may register using the default email verification workflow."
        new EmailParameter{u"email"_s, qtTrId("fskep_ncurses_param_desc_email"), Parameter::Required::No, this}
    };
}

#include "moc_nextcloudusers.cpp"
