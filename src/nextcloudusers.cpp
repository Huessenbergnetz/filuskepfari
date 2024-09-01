#include "nextcloudusers.h"

#include <QCommandLineParser>
#include <QDebug>

using namespace Qt::Literals::StringLiterals;

NextcloudUsers::NextcloudUsers(QCommandLineParser *parser, QObject *parent)
    : Service{parser, parent}
{
}

void NextcloudUsers::initParameters()
{

}

Service::Requirements NextcloudUsers::requirements() const
{
    Service::Requirements reqs;
    reqs.setFlag(Service::Requirement::RemoteUrl);
    reqs.setFlag(Service::Requirement::Auth);
    return reqs;
}

void NextcloudUsers::processData(const std::pair<QStringList, QList<QMap<QString, QString>>> &data)
{
    qDebug() << "Headers:" << data.first;
    qDebug() << "Data:" << data.second;
}

QList<Parameter> NextcloudUsers::parameters() const
{
    QList<Parameter> lst;

    auto &ref = lst.emplace_back(u"uid"_s, Parameter::Type::String, true, qtTrId("fskep_ncu_param_desc_uid"));

    return lst;
}

#include "moc_nextcloudusers.cpp"
