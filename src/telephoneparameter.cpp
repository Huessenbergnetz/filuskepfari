/*
 * SPDX-FileCopyrightText: (C) 2024 Matthias Fehring / www.huessenbergnetz.de
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "telephoneparameter.h"

using namespace Qt::Literals::StringLiterals;

TelephoneParameter::TelephoneParameter(const QString &name, QString description, bool isRequired, QObject *parent)
    : Parameter{name, description, isRequired, parent}
{
    init();
}

bool TelephoneParameter::check(const Row &row) const
{
    if (!Parameter::check(row)) {
        return false;
    }

    const QString value = row.value(name());

    if (value.isEmpty()) {
        return true;
    }

    if (!regex.match(value).hasMatch()) {
        //% "Parameter “%1” is not a valid phone number."
        qCritical().noquote() << qtTrId("fskep_telephoneparameter_check_faild_invalid_phone").arg(value);
        return false;
    }

    return true;
}

QString TelephoneParameter::getValue(const Row &row) const
{
    QString _val = row.value(name());
    _val.replace('+'_L1, u"00"_s);
    _val.remove(QRegularExpression(uR"-([\s\(\)\/-])-"_s));
    return _val;
}

void TelephoneParameter::init()
{
    // regex.setPattern(uR"-((?:([+]\d{1,4})[-.\s]?)?(?:[(](\d{1,3})[)][-.\s]?)?(\d{1,4})[-.\s]?(\d{1,4})[-.\s]?(\d{1,9}))-"_s);
    regex.setPattern(uR"-(^(?:\+|0)[0-9 \(\)\/-]+$)-"_s);
}

#include "moc_telephoneparameter.cpp"
