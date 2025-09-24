// Filename: config_http.h
// Created by:  drose (15Mar09)
//
////////////////////////////////////////////////////////////////////
//
// PANDA 3D SOFTWARE
// Copyright (c) Carnegie Mellon University.  All rights reserved.
//
// All use of this software is subject to the terms of the revised BSD
// license.  You should have received a copy of this license along
// with this source code in a file named "LICENSE."
//
////////////////////////////////////////////////////////////////////
#pragma once
// we only need this if we we are on windows
#ifdef _WIN32
    #pragma comment(lib, "ws2_32.lib")
#endif

#include "pandabase.h"
#include "notifyCategoryProxy.h"

NotifyCategoryDecl(libhttp, EXPORT_CLASS, EXPORT_TEMPL);

extern void init_libhttp();
