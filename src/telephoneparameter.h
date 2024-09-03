/*
 * SPDX-FileCopyrightText: (C) 2024 Matthias Fehring / www.huessenbergnetz.de
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef FSKEP_TELEPHONEPARAMETER_H
#define FSKEP_TELEPHONEPARAMETER_H

#include "parameter.h"

#include "fskep_globals.h"

#include <QObject>
#include <QRegularExpression>

class TelephoneParameter : public Parameter
{
    Q_OBJECT
public:
    TelephoneParameter(const QString &name, QString description, bool isRequired, QObject *parent = nullptr);
    ~TelephoneParameter() override = default;

    [[nodiscard]] bool check(const Row &row) const override;

    [[nodiscard]] QString getValue(const Row &row) const override;

private:
    QRegularExpression regex;

    void init();

    Q_DISABLE_COPY(TelephoneParameter)
};

#endif // FSKEP_TELEPHONEPARAMETER_H
