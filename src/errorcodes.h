/*
 * SPDX-FileCopyrightText: (C) 2024 Matthias Fehring / www.huessenbergnetz.de
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef FSKEP_ERRORCODES_H
#define FSKEP_ERRORCODES_H

namespace Fskep {

enum class ErrCode : int {
    Ok = 0,
    ConfErr = 1,
    AuthErr = 2,
    DataErr = 3,
    ConnErr = 4,
    AppErr  = 5
};

}

#endif // FSKEP_ERRORCODES_H
