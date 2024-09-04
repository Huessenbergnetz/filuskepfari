/*
 * SPDX-FileCopyrightText: (C) 2024 Matthias Fehring / www.huessenbergnetz.de
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "urlparameter.h"

#include <QUrl>

using namespace Qt::Literals::StringLiterals;

UrlParameter::UrlParameter(const QString &name, QString description, bool isRequired, QObject *parent)
    : Parameter{name, description, isRequired, parent}
{
}

bool UrlParameter::check(const Row &row) const
{
    if (!Parameter::check(row)) {
        return false;
    }

    const QString value = row.value(name());

    if (value.isEmpty()) {
        return true;
    }

    QUrl url{value};
    if (!url.isValid()) {
        //% "Parameter “%1” is not a valid URL."
        qCritical().noquote() << qtTrId("fskep_urlparameter_check_faild_invalid_url").arg(value);
        return false;
    }

    return true;
}

#include "moc_urlparameter.cpp"
