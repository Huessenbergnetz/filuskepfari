/*
 * SPDX-FileCopyrightText: (C) 2024 Matthias Fehring / www.huessenbergnetz.de
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef FSKEP_EMAILPARAMETER_H
#define FSKEP_EMAILPARAMETER_H

#include "parameter.h"
#include <QObject>
#include <QRegularExpression>

class EmailParameter : public Parameter
{
    Q_OBJECT
public:
    EmailParameter(const QString &name, QString description, Required required, QObject *parent = nullptr);
    EmailParameter(const QString &name, QString description, Required required, const std::pair<QString,QString> &otherField, QObject *parent = nullptr);
    ~EmailParameter() override = default;

    [[nodiscard]] bool check(const QMap<QString,QString> &data) const override;

private:
    QRegularExpression regex;

    void init();

    Q_DISABLE_COPY(EmailParameter)
};

#endif // FSKEP_EMAILPARAMETER_H
