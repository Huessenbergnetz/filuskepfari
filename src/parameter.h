/*
 * SPDX-FileCopyrightText: (C) 2024 Matthias Fehring / www.huessenbergnetz.de
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef FSKEP_PARAMETER_H
#define FSKEP_PARAMETER_H

#include <QRegularExpression>
#include <QSharedDataPointer>

class Parameter
{
public:
    enum class Type {
        String,
        Url,
        Email,
        Bool,
        Int
    };

    Parameter(const QString &name, Type type, bool required, const QString &description);
    Parameter(const Parameter &other) = default;
    Parameter(Parameter &&other) noexcept = default;
    Parameter &operator=(const Parameter &other) = default;
    Parameter &operator=(Parameter &&other) noexcept = default;
    ~Parameter() noexcept = default;

    void swap(Parameter &other) noexcept
    { data.swap(other.data); }

    [[nodiscard]] QString name() const noexcept;

    [[nodiscard]] Type type() const noexcept;

    [[nodiscard]] bool isRequired() const noexcept;

    [[nodiscard]] QString description() const noexcept;

    [[nodiscard]] QRegularExpression regex() const noexcept;
    void setRegexPattern(const QString &pattern);

private:
    class Data : public QSharedData
    {
    public:
        Data() noexcept = default;
        Data(const QString &_name, Parameter::Type _type, bool _required, const QString &_description);
        Data(const Data &) noexcept = default;
        Data &operator=(const Data &) = delete;
        ~Data() noexcept = default;

        QRegularExpression regex;
        QString name;
        QString description;
        Parameter::Type type{Parameter::Type::String};
        bool required{true};
    };

    QSharedDataPointer<Data> data;
};

Q_DECLARE_SHARED(Parameter)

#endif // FSKEP_PARAMETER_H
