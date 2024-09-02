/*
 * SPDX-FileCopyrightText: (C) 2024 Matthias Fehring / www.huessenbergnetz.de
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef FSKEP_NEXTCLOUDUSERS_H
#define FSKEP_NEXTCLOUDUSERS_H

#include "service.h"

class NextcloudUsers : public Service
{
    Q_OBJECT
public:
    explicit NextcloudUsers(QCommandLineParser *parser, QObject *parent = nullptr);
    ~NextcloudUsers() override = default;

protected:
    void initParameters() override;
    [[nodiscard]] Requirements requirements() const override;
    void processData(const DataPair &data) override;
    QList<Parameter*> parameters() override;
};

#endif // FSKEP_NEXTCLOUDUSERS_H
