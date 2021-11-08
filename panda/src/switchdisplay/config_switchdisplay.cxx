/**
 * PANDA 3D SOFTWARE
 * Copyright (c) Carnegie Mellon University.  All rights reserved.
 *
 * All use of this software is subject to the terms of the revised BSD
 * license.  You should have received a copy of this license along
 * with this source code in a file named "LICENSE."
 *
 * @file config_switchdisplay.cxx
 * @author rdb
 * @date 2013-01-11
 */

#include "config_switchdisplay.h"
#include "switchGraphicsPipe.h"
#include "switchGraphicsWindow.h"
#include "switchGraphicsStateGuardian.h"
#include "graphicsPipeSelection.h"
#include "dconfig.h"
#include "pandaSystem.h"

#include "config_display.h"

Configure(config_switchdisplay);
NotifyCategoryDef(switchdisplay, "display");

ConfigureFn(config_switchdisplay) {
  init_libswitchdisplay();
}

/**
 * Initializes the library.  This must be called at least once before any of
 * the functions or classes in this library can be used.  Normally it will be
 * called by the static initializers and need not be called explicitly, but
 * special cases exist.
 */
void
init_libswitchdisplay() {
  static bool initialized = false;
  if (initialized) {
    return;
  }
  initialized = true;

  init_libdisplay();
  display_cat.get_safe_ptr();

  SwitchGraphicsPipe::init_type();
  SwitchGraphicsWindow::init_type();
  SwitchGraphicsStateGuardian::init_type();

  GraphicsPipeSelection *selection = GraphicsPipeSelection::get_global_ptr();
  selection->add_pipe_type(SwitchGraphicsPipe::get_class_type(),
                           SwitchGraphicsPipe::pipe_constructor);

  PandaSystem *ps = PandaSystem::get_global_ptr();
#ifdef OPENGLES_2
  ps->set_system_tag("OpenGL ES 2", "window_system", "Switch");
#else
  ps->set_system_tag("OpenGL ES", "window_system", "Switch");
#endif
}

/**
 * Returns the given EGL error as string.
 */
const std::string get_egl_error_string(int error) {
  switch (error) {
    case 0x3000: return "EGL_SUCCESS"; break;
    case 0x3001: return "EGL_NOT_INITIALIZED"; break;
    case 0x3002: return "EGL_BAD_ACCESS"; break;
    case 0x3003: return "EGL_BAD_ALLOC"; break;
    case 0x3004: return "EGL_BAD_ATTRIBUTE"; break;
    case 0x3005: return "EGL_BAD_CONFIG"; break;
    case 0x3006: return "EGL_BAD_CONTEXT"; break;
    case 0x3007: return "EGL_BAD_CURRENT_SURFACE"; break;
    case 0x3008: return "EGL_BAD_DISPLAY"; break;
    case 0x3009: return "EGL_BAD_MATCH"; break;
    case 0x300A: return "EGL_BAD_NATIVE_PIXMAP"; break;
    case 0x300B: return "EGL_BAD_NATIVE_WINDOW"; break;
    case 0x300C: return "EGL_BAD_PARAMETER"; break;
    case 0x300D: return "EGL_BAD_SURFACE"; break;
    case 0x300E: return "EGL_CONTEXT_LOST"; break;
    default: return "Unknown error";
  }
}
