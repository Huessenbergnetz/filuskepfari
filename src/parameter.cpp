/*
 * SPDX-FileCopyrightText: (C) 2024 Matthias Fehring / www.huessenbergnetz.de
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "parameter.h"

#include <QCoreApplication>

Parameter::Parameter(const QString &name, QString description, Required required, QObject *parent)
    : QObject{parent}
    , m_name{name}
    , m_description{std::move(description)}
    , m_required{required}
{
}

Parameter::Parameter(const QString &name, QString description, Required required, const std::pair<QString, QString> &otherField, QObject *parent)
    : QObject{parent}
    , m_name{name}
    , m_description{std::move(description)}
    , m_required{required}
    , m_otherField{otherField.first}
    , m_otherValue{otherField.second}
{
}

QString Parameter::name() const noexcept
{
    return m_name;
}

QString Parameter::description() const noexcept
{
    return m_description;
}

Parameter::Required Parameter::required() const noexcept
{
    return m_required;
}

bool Parameter::check(const QMap<QString, QString> &data) const
{
    if (m_required == Required::Yes) {
        if (data.value(m_name).isEmpty()) {
            qCritical().noquote() << qtTrId("Missing parameter %1").arg(name());
            return false;
        }
    } else if (m_required == Required::IfOtherFieldContains ||
               m_required == Required::IfOtherFieldNotConains) {

        const bool isRequired = m_required == Required::IfOtherFieldContains
                ? data.value(m_otherField).contains(m_otherValue)
                : !data.value(m_otherField).contains(m_otherValue);
        if (isRequired && data.value(m_name).isEmpty()) {
            qCritical().noquote() << qtTrId("Missing parameter %1").arg(name());
            return false;
        }
    }

    return true;
}
