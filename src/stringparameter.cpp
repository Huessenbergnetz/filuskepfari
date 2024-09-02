/*
 * SPDX-FileCopyrightText: (C) 2024 Matthias Fehring / www.huessenbergnetz.de
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "stringparameter.h"

#include <QDebug>
#include <QRegularExpression>

StringParameter::StringParameter(const QString &name, QString description, Required required, const QString &pattern, QObject *parent)
    : Parameter{name, description, required, parent}
{
    if (!pattern.isEmpty()) {
        regex = std::make_unique<QRegularExpression>(pattern);
    }
}

StringParameter::StringParameter(const QString &name, QString description, Required required, const std::pair<QString,QString> &otherField, const QString &pattern, QObject *parent)
    : Parameter{name, description, required, otherField, parent}
{
    if (!pattern.isEmpty()) {
        regex = std::make_unique<QRegularExpression>(pattern);
    }
}

bool StringParameter::check(const QMap<QString, QString> &data) const
{
    if (!Parameter::check(data)) {
        return false;
    }

    const QString value = data.value(name());

    if (value.isEmpty()) {
        return true;
    }

    if (regex && !regex->match(value).hasMatch()) {
        qCritical().noquote() << qtTrId("Parameter “%1” does not match regular expression pattern “%2”").arg(name(), regex->pattern());
        return false;
    }

    return true;
}
