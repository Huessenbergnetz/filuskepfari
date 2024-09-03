/*
 * SPDX-FileCopyrightText: (C) 2024 Matthias Fehring / www.huessenbergnetz.de
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef FSKEP_STRINGPARAMETER_H
#define FSKEP_STRINGPARAMETER_H

#include "parameter.h"

#include "fskep_globals.h"

class QRegularExpression;

class StringParameter : public Parameter
{
    Q_OBJECT
public:
    StringParameter(const QString &name, QString description, bool isRequired, QObject *parent = nullptr);
    StringParameter(const QString &name, QString description, bool isRequired, const QString &pattern = {}, QObject *parent = nullptr);
    ~StringParameter() override = default;

    [[nodiscard]] bool check(const Row &row) const override;

private:
    std::unique_ptr<QRegularExpression> regex{nullptr};

    Q_DISABLE_COPY(StringParameter)
};

#endif // FSKEP_STRINGPARAMETER_H
