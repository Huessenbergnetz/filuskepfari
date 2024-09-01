# SPDX-FileCopyrightText: (C) 2024 Matthias Fehring / www.huessenbergnetz.de
# SPDX-License-Identifier: BSD-3-Clause

set(filuskepfari_TS_FILES
    filuskepfari.ts
    filuskepfari_de.ts
)

qt_add_lupdate(filuskepfari
    TS_FILES ${filuskepfari_TS_FILES}
    OPTIONS -no-obsolete -locations none
)

qt_add_lrelease(filuskepfari
    TS_FILES ${filuskepfari_TS_FILES}
    QM_FILES_OUTPUT_VARIABLE filuskepfari_QM_FILES
    OPTIONS -idbased
)

install(FILES ${filuskepfari_QM_FILES} DESTINATION ${FSKEP_TRANS_DIR})
