/*
 * SPDX-FileCopyrightText: (C) 2024 Matthias Fehring / www.huessenbergnetz.de
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef FSKEP_PARAMETER_H
#define FSKEP_PARAMETER_H

#include <QObject>
#include <QVariant>

class Parameter : public QObject
{
    Q_OBJECT
public:
    enum class Required {
        No,
        Yes,
        IfOtherFieldContains,
        IfOtherFieldNotConains
    };

    Parameter(const QString &name, QString description, Required required, QObject *parent = nullptr);
    Parameter(const QString &name, QString description, Required required, const std::pair<QString,QString> &otherField, QObject *parent = nullptr);
    ~Parameter() override = default;

    [[nodiscard]] QString name() const noexcept;

    [[nodiscard]] QString description() const noexcept;

    [[nodiscard]] Required required() const noexcept;

    [[nodiscard]] virtual bool check(const QMap<QString, QString> &data) const;

private:
    QString m_name;
    QString m_description;
    QString m_otherField;
    QString m_otherValue;
    Parameter::Required m_required{Parameter::Required::No};

    Q_DISABLE_COPY(Parameter)
};

#endif // FSKEP_PARAMETER_H
