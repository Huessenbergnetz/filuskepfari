/*
 * SPDX-FileCopyrightText: (C) 2024 Matthias Fehring / www.huessenbergnetz.de
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef FSKEP_NEXTCLOUDUSERS_H
#define FSKEP_NEXTCLOUDUSERS_H

#include "service.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkRequest>
#include <QUrl>

class QNetworkReply;
class QNetworkRequest;
class QJsonParseError;

using namespace Fskep;

class NextcloudUsers : public Service
{
    Q_OBJECT
public:
    explicit NextcloudUsers(QCommandLineParser *parser, QObject *parent = nullptr);
    ~NextcloudUsers() override = default;

protected:
    [[nodiscard]] Requirements requirements() const override;
    void processData() override;
    QList<Parameter*> parameters() override;

private:
    void startCreateUser();
    void prepareSettingUserData();
    void startSetUserDetails();
    [[nodiscard]] QUrl createUrl(const QString &path = {}) const;
    [[nodiscard]] QNetworkRequest createRequest(const QUrl &url) const;
    [[nodiscard]] QJsonDocument parseJsonReply(const QByteArray &data) const;
    [[nodiscard]] QJsonObject checkOcsObject(const QJsonDocument &json, bool *ok) const;

    Row m_current;

    QQueue<QString> m_userDetailKeys;

private slots:
    void onCreateUserFinished(QNetworkReply *reply);
    void onSetUserDetailsFinished(QNetworkReply *reply);
};

#endif // FSKEP_NEXTCLOUDUSERS_H
