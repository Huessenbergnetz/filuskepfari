/*
 * SPDX-FileCopyrightText: (C) 2024 Matthias Fehring / www.huessenbergnetz.de
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef FSKEP_SERVICE_H
#define FSKEP_SERVICE_H

#include "parameter.h"

#include <QFileInfo>
#include <QObject>
#include <QList>
#include <QUrl>

class QCommandLineParser;

class Service : public QObject
{
    Q_OBJECT
public:
    using DataPair = std::pair<QStringList, QList<QMap<QString, QString>>>;

    enum class Requirement {
        Nothing     = 0x0000,
        RemoteUrl   = 0x0001,
        Auth        = 0x0002
    };
    Q_DECLARE_FLAGS(Requirements, Requirement)
    Q_FLAG(Requirements)

    explicit Service(QCommandLineParser *parser, QObject *parent = nullptr);
    ~Service() override = default;

    void start();

    static QStringList supportedMimeType();

protected:
    [[nodiscard]] QCommandLineParser *parser() const;
    virtual void initParameters() = 0;
    [[nodiscard]] virtual Requirements requirements() const = 0;
    virtual void processData(const std::pair<QStringList, QList<QMap<QString, QString>>> &data) = 0;
    virtual QList<Parameter> parameters() const = 0;

private slots:
    void doStart();

private:
    bool checkRequirements();
    [[nodiscard]] std::pair<QStringList, QList<QMap<QString, QString>>> readInputFile() const;
    [[nodiscard]] std::pair<QStringList, QList<QMap<QString, QString>>> readInputCsvFile() const;
    bool checkData(const DataPair &data) const;

    QList<Parameter> m_parameters;
    QFileInfo m_inputFileInfo;
    QString m_inputFileMimeType;
    QString m_username;
    QString m_password;
    QUrl m_url;
    QCommandLineParser *m_parser{nullptr};

    static const QStringList m_supportedMimeTypes;

    Q_DISABLE_COPY(Service)
};

#endif // FSKEP_SERVICE_H
