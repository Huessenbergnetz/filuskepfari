/*
 * SPDX-FileCopyrightText: (C) 2024 Matthias Fehring / www.huessenbergnetz.de
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "parameter.h"

#include <QCoreApplication>

Parameter::Parameter(const QString &name, QString description, bool isRequired, QObject *parent)
    : QObject{parent}
    , m_name{name}
    , m_description{std::move(description)}
    , m_isRequired{isRequired}
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

bool Parameter::isRequired() const noexcept
{
    return m_isRequired;
}

bool Parameter::check(const Row &row) const
{
    if (m_isRequired && row.value(m_name).isEmpty()) {
        return false;
    }

    return true;
}

QString Parameter::getValue(const Row &row) const
{
    return row.value(m_name);
}
