/*
 * SPDX-FileCopyrightText: (C) 2024 Matthias Fehring / www.huessenbergnetz.de
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef FSKEP_PARAMETER_H
#define FSKEP_PARAMETER_H

#include "fskep_globals.h"

#include <QObject>
#include <QVariant>

using namespace Fskep;

class Parameter : public QObject
{
    Q_OBJECT
public:
    Parameter(const QString &name, QString description, bool isRequired, QObject *parent = nullptr);
    ~Parameter() override = default;

    [[nodiscard]] QString name() const noexcept;

    [[nodiscard]] QString description() const noexcept;

    [[nodiscard]] bool isRequired() const noexcept;

    [[nodiscard]] virtual bool check(const Row &row) const;

    [[nodiscard]] virtual QVariant getValue(const Row &row) const;

private:
    QString m_name;
    QString m_description;
    QString m_otherField;
    QString m_otherValue;
    bool m_isRequired{false};

    Q_DISABLE_COPY(Parameter)
};

#endif // FSKEP_PARAMETER_H
