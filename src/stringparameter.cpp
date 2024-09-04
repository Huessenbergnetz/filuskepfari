/*
 * SPDX-FileCopyrightText: (C) 2024 Matthias Fehring / www.huessenbergnetz.de
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "stringparameter.h"

#include <QDebug>
#include <QRegularExpression>

StringParameter::StringParameter(const QString &name, QString description, bool isRequired, QObject *parent)
    : Parameter{name, description, isRequired, parent}
{
}

StringParameter::StringParameter(const QString &name, QString description, bool isRequired, const QString &pattern, QObject *parent)
    : Parameter{name, description, isRequired, parent}
{
    if (!pattern.isEmpty()) {
        regex = std::make_unique<QRegularExpression>(pattern);
    }
}

bool StringParameter::check(const Row &row) const
{
    if (!Parameter::check(row)) {
        return false;
    }

    const QString value = row.value(name());

    if (value.isEmpty()) {
        return true;
    }

    if (regex && !regex->match(value).hasMatch()) {
        //% "Parameter “%1” does not match regular expression pattern “%2”."
        qCritical().noquote() << qtTrId("fskep_stringparameter_check_failed_regex").arg(value, regex->pattern());
        return false;
    }

    return true;
}
