/*
 * SPDX-FileCopyrightText: (C) 2024 Matthias Fehring / www.huessenbergnetz.de
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef FSKEP_URLPARAMETER_H
#define FSKEP_URLPARAMETER_H

#include "parameter.h"
#include <QObject>

class UrlParameter : public Parameter
{
    Q_OBJECT
public:
    UrlParameter(const QString &name, QString description, bool isRequired, QObject *parent = nullptr);
    ~UrlParameter() override = default;

    [[nodiscard]] bool check(const Row &row) const override;

private:
    Q_DISABLE_COPY(UrlParameter)
};

#endif // FSKEP_URLPARAMETER_H
