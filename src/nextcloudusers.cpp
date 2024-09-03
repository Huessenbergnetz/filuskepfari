/*
 * SPDX-FileCopyrightText: (C) 2024 Matthias Fehring / www.huessenbergnetz.de
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "nextcloudusers.h"

#include "emailparameter.h"
#include "stringparameter.h"
#include "telephoneparameter.h"

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

void NextcloudUsers::processData()
{
    qDebug() << "Headers:" << headers();
    qDebug() << "Data:" << rows();
}

QList<Parameter*> NextcloudUsers::parameters()
{
    return {
        //% "Account ID used to login (must only contain a-z, A-Z, 0-9, -, _ and @)"
        new StringParameter{u"uid"_s, qtTrId("fskep_ncusers_param_desc_uid"), true, u"^[a-zA-Z0-9_@\\.-]+$"_s, this},
        //% "Display name for the new user. Can conain any character."
        new StringParameter{u"displayname"_s, qtTrId("fskep_ncusers_param_desc_displayName"), false, this},
        //% "When set, users may register using the default email verification workflow."
        new EmailParameter{u"email"_s, qtTrId("fskep_ncusers_param_desc_email"), false, this},
        //% "Groups the account should be added to. (The group will be created if it does not exist.)"
        new StringParameter{u"groups"_s, qtTrId("fskep_ncusers_param_desc_groups"), false, this},
        //% "Language of the user."
        new StringParameter{u"language"_s, qtTrId("fksep_ncusers_param_desc_language"), false, u"^([a-z]{2})(_([A-Z]{2}))?$"_s, this},
        new StringParameter{u"password"_s, qtTrId("fskep_ncusers_param_desc_password"), false, this},
        new EmailParameter{u"additional_email"_s, qtTrId("fskep_ncusers_param_desc_addemail"), false, this},
        new TelephoneParameter{u"phone"_s, qtTrId("fskep_ncusers_param_desc_phone"), false, this},
        new StringParameter{u"address"_s, qtTrId("fskep_ncusers_param_desc_address"), false, this},
        new StringParameter{u"organisation"_s, qtTrId("fskep_ncusers_param_desc_organisation"), false, this},
        new StringParameter{u"role"_s, qtTrId("fskep_ncusers_param_desc_role"), false, this}
    };
}

#include "moc_nextcloudusers.cpp"
