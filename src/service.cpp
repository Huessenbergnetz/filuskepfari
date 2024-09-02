/*
 * SPDX-FileCopyrightText: (C) 2024 Matthias Fehring / www.huessenbergnetz.de
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "service.h"

#include "errorcodes.h"
#include "parameter.h"

#include <QCommandLineParser>
#include <QCoreApplication>
#include <QDir>
#include <QLocale>
#include <QMimeDatabase>
#include <QMimeType>
#include <QTextStream>
#include <QTimer>

using namespace Qt::Literals::StringLiterals;

const QStringList Service::m_supportedMimeTypes = QStringList{
        u"text/csv"_s
};

Service::Service(QCommandLineParser *parser, QObject *parent)
    : QObject{parent}
    , m_parser{parser}
{

}

void Service::start()
{
    QTimer::singleShot(0, this, &Service::doStart);
}

void Service::doStart()
{
    if (!checkRequirements()) {
        return;
    }

    const DataPair data = readInputFile();
    if (data.first.isEmpty()) {
        return;
    }

    if (!checkData(data)) {
        return;
    }

    processData(data);
}

bool Service::checkRequirements()
{
    const auto reqs = requirements();

    if (reqs.testFlag(Service::Requirement::RemoteUrl)) {
        const QString remote = m_parser->value(u"remote"_s);
        if (remote.isEmpty()) {
            //% "Missing remote URL. Use the %1 option to specify the remote URL."
            qCritical().noquote() << qtTrId("fskep_err_missing_remote").arg(u"--remote"_s);
            QCoreApplication::exit(static_cast<int>(Fskep::ErrCode::ConfErr));
            return false;
        }

        if (const QUrl url = QUrl(remote); !url.isValid() || !url.scheme().startsWith("http"_L1)) {
            //% "Invalid remote URL. Has to start with http:// or https://."
            qCritical().noquote() << qtTrId("fskep_err_invalid_remote_url");
            QCoreApplication::exit(static_cast<int>(Fskep::ErrCode::ConfErr));
            return false;
        }
    }

    if (reqs.testFlag(Service::Requirement::Auth)) {

    }

    const QString inputFilePath = m_parser->value(u"input"_s);
    if (inputFilePath.isEmpty()) {
        //% "Missing path to input file. Use the %1 option to specify the input file path."
        qCritical().noquote() << qtTrId("fskep_err_missing_input").arg(u"--input"_s);
        QCoreApplication::exit(static_cast<int>(Fskep::ErrCode::ConfErr));
        return false;
    }

    m_inputFileInfo.setFile(inputFilePath);
    if (!m_inputFileInfo.exists()) {
        //% "Can not find input file."
        qCritical().noquote() << qtTrId("fskep_err_input_file_not_found");
        QCoreApplication::exit(static_cast<int>(Fskep::ErrCode::ConfErr));
        return false;
    }

    if (!m_inputFileInfo.isFile()) {
        //% "Input is not a file."
        qCritical().noquote() << qtTrId("fskep_err_input_not_a_file");
        QCoreApplication::exit(static_cast<int>(Fskep::ErrCode::ConfErr));
        return false;
    }

    if (!m_inputFileInfo.isReadable()) {
        //% "Input file is not readable."
        qCritical().noquote() << qtTrId("fskep_err_input_not_readable");
        QCoreApplication::exit(static_cast<int>(Fskep::ErrCode::DataErr));
        return false;
    }

    if (m_inputFileInfo.size() == 0) {
        //% "Emtpy input file."
        qCritical().noquote() << qtTrId("fskep_err_input_empty");
        QCoreApplication::exit(static_cast<int>(Fskep::ErrCode::DataErr));
        return false;
    }

    QMimeDatabase mimeDb;
    const QMimeType mt = mimeDb.mimeTypeForFile(m_inputFileInfo.absoluteFilePath());
    if (!m_supportedMimeTypes.contains(mt.name())) {
        const QLocale locale;
        //% "Input files of type %1 are not supported.\nSupported types: %2"
        qCritical().noquote() << qtTrId("fskep_err_unsupported_mimetype").arg(mt.name(), locale.createSeparatedList(m_supportedMimeTypes));
        QCoreApplication::exit(static_cast<int>(Fskep::ErrCode::DataErr));
        return false;
    }
    m_inputFileMimeType = mt.name();

    return true;
}

QCommandLineParser* Service::parser() const
{
    return m_parser;
}

std::pair<QStringList,QList<QMap<QString,QString>>> Service::readInputFile() const
{
    if (m_inputFileMimeType == "text/csv"_L1) {
        return readInputCsvFile();
    }

    return {};
}

std::pair<QStringList,QList<QMap<QString,QString>>> Service::readInputCsvFile() const
{
    QFile f{m_inputFileInfo.absoluteFilePath()};

    if (!f.open(QFile::ReadOnly|QFile::Text)) {
        //% "Failed to open input file: %1"
        qCritical().noquote() << qtTrId("fskep_err_open_file").arg(f.errorString());
        QCoreApplication::exit(static_cast<int>(Fskep::ErrCode::DataErr));
        return {};
    }

    QStringList headers;
    QList<QMap<QString,QString>> rows;

    QTextStream stream(&f);
    int row = 0;
    while (!stream.atEnd()) {
        const QString line = stream.readLine();
        const QStringList data = line.split(QLatin1Char(','));
        if (row == 0) {
            headers = data;
        } else {
            if (headers.size() != data.size()) {
                //% "Data at line %1 has not the same columnt count as the number of header fields. "
                //% "Expected: %2, Actual: %3"
                qCritical().noquote() << qtTrId("fskep_err_data_row_size_mismatch").arg(QString::number(row + 1), QString::number(headers.size()), QString::number(data.size()));
                QCoreApplication::exit(static_cast<int>(Fskep::ErrCode::DataErr));
                return {};
            }

            QMap<QString,QString> rowData;
            for (int col = 0; col < headers.size(); ++col) {
                rowData.insert(headers.at(col), data.at(col));
            }
            rows << rowData;
        }
        row++;
    }

    return std::make_pair(headers, rows);
}

bool Service::checkData(const DataPair &data)
{
    const QList<Parameter*> params = parameters();

    const QStringList headers = data.first;

    for (const auto p : params) {
        if (p->required() == Parameter::Required::Yes && !headers.contains(p->name())) {
            qCritical().noquote() << qtTrId("Required parameter “%1” is not available in the header data.").arg(p->name());
            QCoreApplication::exit(static_cast<int>(Fskep::ErrCode::DataErr));
            return false;
        }
    }

    const RowList rows = data.second;

    int line = 0;
    for (const Row &row : rows) {
        for (const auto p : params) {
            if (!p->check(row)) {
                qCritical().noquote() << qtTrId("Invalid data for column “%1” at line %2.").arg(p->name(), QString::number(line));
                return false;
            }
        }
        line++;
    }

    return true;
}

#include "moc_service.cpp"
