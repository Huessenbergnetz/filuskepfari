#ifndef FSKEP_GLOBALS_H
#define FSKEP_GLOBALS_H

#include <QList>
#include <QMap>
#include <QString>

#include <utility>

namespace Fskep {

enum class ErrCode : int {
    Ok = 0,
    ConfErr = 1,
    AuthErr = 2,
    DataErr = 3,
    ConnErr = 4,
    AppErr  = 5
};

using Row = QMap<QString,QString>;
using RowList = QList<Row>;
using DataPair = std::pair<QStringList, RowList>;

}

#endif // FSKEP_GLOBALS_H
