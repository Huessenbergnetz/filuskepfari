# SPDX-FileCopyrightText: (C) 2024 Matthias Fehring / www.huessenbergnetz.de
# SPDX-License-Identifier: BSD-3-Clause

add_executable(filuskepfari)

target_sources(filuskepfari
    PRIVATE
        main.cpp
        fskep_globals.h
        parameter.cpp
        parameter.h
        service.cpp
        service.h
        nextcloudusers.cpp
        nextcloudusers.h
        stringparameter.cpp
        stringparameter.h
        emailparameter.cpp
        emailparameter.h
        telephoneparameter.cpp
        telephoneparameter.h
)

target_link_libraries(filuskepfari
    PRIVATE
        Qt${QT_VERSION_MAJOR}::Core
        Qt${QT_VERSION_MAJOR}::Network
)

target_compile_definitions(filuskepfari
    PRIVATE
        QT_NO_CAST_TO_ASCII
        QT_NO_CAST_FROM_ASCII
        QT_STRICT_ITERATORS
        QT_NO_URL_CAST_FROM_STRING
        QT_NO_CAST_FROM_BYTEARRAY
        QT_USE_QSTRINGBUILDER
        QT_USE_FAST_OPERATOR_PLUS
        FSKEP_VERSION="${PROJECT_VERSION}"
        FSKEP_TRANS_DIR="${FSKEP_TRANS_FULL_DIR}"
        $<$<NOT:$<CONFIG:Debug>>:QT_NO_DEBUG_OUTPUT>
)

install(TARGETS filuskepfari DESTINATION ${CMAKE_INSTALL_BINDIR})
