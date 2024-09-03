/*
 * SPDX-FileCopyrightText: (C) 2024 Matthias Fehring / www.huessenbergnetz.de
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "emailparameter.h"

#include <QDebug>

using namespace Qt::Literals::StringLiterals;

EmailParameter::EmailParameter(const QString &name, QString description, bool isRequired, QObject *parent)
    : Parameter{name, description, isRequired, parent}
{
    init();
}

bool EmailParameter::check(const Row &row) const
{
    if (!Parameter::check(row)) {
        return false;
    }

    const QString value = row.value(name());

    if (value.isEmpty()) {
        return true;
    }

    if (!regex.match(value).hasMatch()) {
        qCritical().noquote() << qtTrId("Parameter “%1“ is not a valid email address.").arg(value);
        return false;
    }

    return true;
}

void EmailParameter::init()
{
    regex.setPattern(uR"-([a-zA-Z0-9.!#$%&’*+\/=?^_`{|}~-]+@[a-zA-Z0-9-]+(?:\.[a-zA-Z0-9-]+)*)-"_s);
//     regex.setPattern(uR"-((?(DEFINE)
//     (?<addr_spec> (?&local_part) @ (?&domain) )
//     (?<local_part> (?&dot_atom) | (?&quoted_string) | (?&obs_local_part) )
//     (?<domain> (?&dot_atom) | (?&domain_literal) | (?&obs_domain) )
//     (?<domain_literal> (?&CFWS)? \[ (?: (?&FWS)? (?&dtext) )* (?&FWS)? \] (?&CFWS)? )
//     (?<dtext> [\x21-\x5a] | [\x5e-\x7e] | (?&obs_dtext) )
//     (?<quoted_pair> \\ (?: (?&VCHAR) | (?&WSP) ) | (?&obs_qp) )
//     (?<dot_atom> (?&CFWS)? (?&dot_atom_text) (?&CFWS)? )
//     (?<dot_atom_text> (?&atext) (?: \. (?&atext) )* )
//     (?<atext> [a-zA-Z0-9!#$%&'*+\/=?^_`{|}~-]+ )
//     (?<atom> (?&CFWS)? (?&atext) (?&CFWS)? )
//     (?<word> (?&atom) | (?&quoted_string) )
//     (?<quoted_string> (?&CFWS)? " (?: (?&FWS)? (?&qcontent) )* (?&FWS)? " (?&CFWS)? )
//     (?<qcontent> (?&qtext) | (?&quoted_pair) )
//     (?<qtext> \x21 | [\x23-\x5b] | [\x5d-\x7e] | (?&obs_qtext) )
//     # comments and whitespace
//     (?<FWS> (?: (?&WSP)* \r\n )? (?&WSP)+ | (?&obs_FWS) )
//     (?<CFWS> (?: (?&FWS)? (?&comment) )+ (?&FWS)? | (?&FWS) )
//     (?<comment> \( (?: (?&FWS)? (?&ccontent) )* (?&FWS)? \) )
//     (?<ccontent> (?&ctext) | (?&quoted_pair) | (?&comment) )
//     (?<ctext> [\x21-\x27] | [\x2a-\x5b] | [\x5d-\x7e] | (?&obs_ctext) )
//     # obsolete tokens
//     (?<obs_domain> (?&atom) (?: \. (?&atom) )* )
//     (?<obs_local_part> (?&word) (?: \. (?&word) )* )
//     (?<obs_dtext> (?&obs_NO_WS_CTL) | (?&quoted_pair) )
//     (?<obs_qp> \\ (?: \x00 | (?&obs_NO_WS_CTL) | \n | \r ) )
//     (?<obs_FWS> (?&WSP)+ (?: \r\n (?&WSP)+ )* )
//     (?<obs_ctext> (?&obs_NO_WS_CTL) )
//     (?<obs_qtext> (?&obs_NO_WS_CTL) )
//     (?<obs_NO_WS_CTL> [\x01-\x08] | \x0b | \x0c | [\x0e-\x1f] | \x7f )
//     # character class definitions
//     (?<VCHAR> [\x21-\x7E] )
//     (?<WSP> [ \t] )
// )
// ^(?&addr_spec)$)-"_s);
}
