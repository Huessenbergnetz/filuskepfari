/*
 * SPDX-FileCopyrightText: (C) 2024 Matthias Fehring / www.huessenbergnetz.de
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "parameter.h"

Parameter::Data::Data(const QString &_name, Parameter::Type _type, bool _required, const QString &_description)
    : QSharedData()
    , name{_name}
    , description{_description}
    , type{_type}
    , required{_required}
{
}

Parameter::Parameter(const QString &name, Type type, bool required, const QString &description)
    : data{new Data{name, type, required, description}}
{
}

QString Parameter::name() const noexcept
{
    return data->name;
}

Parameter::Type Parameter::type() const noexcept
{
    return data->type;
}

bool Parameter::isRequired() const noexcept
{
    return data->required;
}

QString Parameter::description() const noexcept
{
    return data->description;
}
