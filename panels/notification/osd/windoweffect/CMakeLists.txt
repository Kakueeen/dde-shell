# SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
#
# SPDX-License-Identifier: GPL-3.0-or-later

add_library(osd-windowEffect SHARED
    windoweffectapplet.cpp
    windoweffectapplet.h
)

target_link_libraries(osd-windowEffect PRIVATE
    dde-shell-frame
)

ds_install_package(PACKAGE org.deepin.ds.osd.windoweffect TARGET osd-windowEffect)
ds_handle_package_translation(PACKAGE org.deepin.ds.osd.windoweffect)
