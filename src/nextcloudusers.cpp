/*
 * SPDX-FileCopyrightText: (C) 2024 Matthias Fehring / www.huessenbergnetz.de
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "nextcloudusers.h"

#include "emailparameter.h"
#include "fskep_globals.h"
#include "stringparameter.h"
#include "telephoneparameter.h"

#include <QCommandLineParser>
#include <QDebug>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QTimer>
#include <QUrlQuery>

using namespace Qt::Literals::StringLiterals;

NextcloudUsers::NextcloudUsers(QCommandLineParser *parser, QObject *parent)
    : Service{parser, parent}
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
    startCreateUser();
}

void NextcloudUsers::startCreateUser()
{
    if (m_rows.empty()) {
        QCoreApplication::quit();
        return;
    }

    m_current = m_rows.dequeue();

    //% "Creating new user “%1”"
    qInfo().noquote() << qtTrId("fskep_ncusers_msg_info_start_reate_user").arg(m_current.value(u"userid"_s));

    QUrlQuery query;
    for (const QString &p : {u"userid"_s, u"displayname"_s, u"email"_s, u"groups"_s, u"language"_s, u"password"_s}) {
        const QString val = m_current.value(p);
        if (!val.isEmpty()) {
            if (p != "groups"_L1) {
                query.addQueryItem(p, val);
            } else {
                const auto groups = val.split(listSeparator(), Qt::SkipEmptyParts);
                for (const QString &g : groups) {
                    query.addQueryItem(u"groups[]"_s, g);
                }
            }
        }
    }

    QUrl url = createUrl(u"/ocs/v2.php/cloud/users"_s);
    url.setQuery(query);

    const auto req = createRequest(url);

    auto nam = new QNetworkAccessManager(this);
    connect(nam, &QNetworkAccessManager::finished, this, &NextcloudUsers::onCreateUserFinished);
    nam->post(req, QByteArray());
}

void NextcloudUsers::onCreateUserFinished(QNetworkReply *reply)
{
    if (reply->error() == QNetworkReply::NoError) {
        const QByteArray jsonReply = reply->readAll();
        reply->manager()->deleteLater();
        reply->deleteLater();

        QJsonParseError jpe;
        const QJsonDocument json = QJsonDocument::fromJson(jsonReply, &jpe);
        if (jpe.error != QJsonParseError::NoError) {
            qCritical() << jpe.errorString();
            QCoreApplication::exit(static_cast<int>(ErrCode::AppErr));
            return;
        }

        const auto ocs = json.object().value("ocs"_L1).toObject();
        const auto meta = ocs.value("meta"_L1).toObject();
        if (meta.value("status"_L1).toString() == "ok"_L1) {
            const auto data = ocs.value("data"_L1).toObject();
            //% "Successfully created new user “%1”"
            qInfo().noquote() << qtTrId("fskep_ncusers_msg_success_create_user").arg(data.value("id"_L1).toString());
            prepareSettingUserData();
        } else {
            //% "Failed to create new user: %1"
            qCritical().noquote() << qtTrId("fskep_ncusers_msg_err_create_user").arg(meta.value("message"_L1).toString());
            QCoreApplication::exit(static_cast<int>(ErrCode::DataErr));
        }
    } else {
        qCritical() << reply->errorString();
        QCoreApplication::exit(static_cast<int>(ErrCode::ConnErr));
    }
}

void NextcloudUsers::prepareSettingUserData()
{
    m_userDetailKeys.clear();
    for (const QString &key : {u"additional_email"_s, u"phone"_s, u"address"_s, u"website"_s, u"twitter"_s, u"fediverse"_s,
         u"organisation"_s, u"role"_s, u"headline"_s, u"biography"_s, u"profile_enabled"_s}) {
        if (m_current.contains(key)) {
            m_userDetailKeys.enqueue(key);
        }
    }

    if (m_userDetailKeys.empty()) {
        //% "No additional user fields found. Skipping setting user details."
        qInfo().noquote() << qtTrId("fskep_ncusers_msg_info_no_user_details");
        startCreateUser();
    } else {
        startSetUserDetails();
    }
}

void NextcloudUsers::startSetUserDetails()
{
    if (m_userDetailKeys.empty()) {
        startCreateUser();
        return;
    }

    const QString key = m_userDetailKeys.dequeue();
    const QString uid = m_current.value(u"userid"_s);

    //% "Start setting “%1” for user “%2”
    qInfo().noquote() << qtTrId("fskep_ncusers_msg_info_start_set_user_detail").arg(key, uid);

    QUrlQuery query;
    query.addQueryItem(u"key"_s, key);
    query.addQueryItem(u"value"_s, m_current.value(key));

    QUrl url = createUrl(u"/ocs/v2.php/cloud/users/"_s + uid);
    url.setQuery(query);

    const auto req = createRequest(url);

    auto nam = new QNetworkAccessManager(this);
    connect(nam, &QNetworkAccessManager::finished, this, &NextcloudUsers::onCreateUserFinished);
    nam->put(req, QByteArray());
}

void NextcloudUsers::onSetUserDetailsFinished(QNetworkReply *reply)
{
    if (reply->error() == QNetworkReply::NoError) {
        const QByteArray jsonReply = reply->readAll();
        reply->manager()->deleteLater();
        reply->deleteLater();
        qDebug() << jsonReply;
        startSetUserDetails();
    } else {
        qCritical() << reply->errorString();
        QCoreApplication::exit(static_cast<int>(ErrCode::ConnErr));
    }
}

QList<Parameter*> NextcloudUsers::parameters()
{
    return {
        //% "Account ID used to login (must only contain a-z, A-Z, 0-9, -, _ and @)"
        new StringParameter{u"userid"_s, qtTrId("fskep_ncusers_param_desc_uid"), true, u"^[a-zA-Z0-9_@\\.-]+$"_s, this},
        //% "Display name for the new user. Can conain any character."
        new StringParameter{u"displayname"_s, qtTrId("fskep_ncusers_param_desc_displayName"), false, this},
        //% "When set, users may register using the default email verification workflow."
        new EmailParameter{u"email"_s, qtTrId("fskep_ncusers_param_desc_email"), false, this},
        //% "Groups the account should be added to."
        new StringParameter{u"groups"_s, qtTrId("fskep_ncusers_param_desc_groups"), false, this},
        //% "Language of the user."
        new StringParameter{u"language"_s, qtTrId("fksep_ncusers_param_desc_language"), false, u"^([a-z]{2})(_([A-Z]{2}))?$"_s, this},
        new StringParameter{u"password"_s, qtTrId("fskep_ncusers_param_desc_password"), false, this},
        new EmailParameter{u"additional_mail"_s, qtTrId("fskep_ncusers_param_desc_addemail"), false, this},
        new TelephoneParameter{u"phone"_s, qtTrId("fskep_ncusers_param_desc_phone"), false, this},
        new StringParameter{u"address"_s, qtTrId("fskep_ncusers_param_desc_address"), false, this},
        new StringParameter{u"organisation"_s, qtTrId("fskep_ncusers_param_desc_organisation"), false, this},
        new StringParameter{u"role"_s, qtTrId("fskep_ncusers_param_desc_role"), false, this}
    };
}

QUrl NextcloudUsers::createUrl(const QString &path) const
{
    QUrl url{parser()->value(u"remote"_s)};
    if (!path.isEmpty()) {
        QString _path = url.path();
        if (_path.endsWith('/'_L1)) {
            _path.chop(1);
        }
        _path.append(path);
        url.setPath(_path);
    }
    return url;
}

QNetworkRequest NextcloudUsers::createRequest(const QUrl &url) const
{
    QNetworkRequest req{url};

    req.setRawHeader("OCS-APIRequest"_ba, "true"_ba);
    req.setRawHeader("Accept"_ba, "application/json"_ba);
    req.setRawHeader("Content-Type"_ba, "application/json"_ba);

    const QString auth = parser()->value(u"user"_s) + ':'_L1 + parser()->value(u"password"_s);
    const QByteArray authHeader = "Basic "_ba + auth.toUtf8().toBase64();
    req.setRawHeader("Authorization"_ba, authHeader);

    return req;
}

#include "moc_nextcloudusers.cpp"
