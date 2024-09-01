#ifndef NEXTCLOUDUSERS_H
#define NEXTCLOUDUSERS_H

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
    void processData(const std::pair<QStringList, QList<QMap<QString, QString>>> &data) override;
    QList<Parameter> parameters() const override;
};

#endif // NEXTCLOUDUSERS_H
