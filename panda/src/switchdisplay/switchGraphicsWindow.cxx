/**
 * PANDA 3D SOFTWARE
 * Copyright (c) Carnegie Mellon University.  All rights reserved.
 *
 * All use of this software is subject to the terms of the revised BSD
 * license.  You should have received a copy of this license along
 * with this source code in a file named "LICENSE."
 *
 * @file switchGraphicsWindow.cxx
 * @author rdb
 * @date 2013-01-11
 */

#include "switchGraphicsWindow.h"
#include "switchGraphicsStateGuardian.h"
#include "config_switchdisplay.h"
#include "switchGraphicsPipe.h"
#include "libnxWrapper.h"

#include "graphicsPipe.h"
#include "keyboardButton.h"
#include "mouseButton.h"
#include "clockObject.h"
#include "pStatTimer.h"
#include "textEncoder.h"
#include "throw_event.h"
#include "nativeWindowHandle.h"

TypeHandle SwitchGraphicsWindow::_type_handle;

/**
 *
 */
SwitchGraphicsWindow::
SwitchGraphicsWindow(GraphicsEngine *engine, GraphicsPipe *pipe,
                      const std::string &name,
                      const FrameBufferProperties &fb_prop,
                      const WindowProperties &win_prop,
                      int flags,
                      GraphicsStateGuardian *gsg,
                      GraphicsOutput *host) :
  GraphicsWindow(engine, pipe, name, fb_prop, win_prop, flags, gsg, host),
  _mouse_button_state(0)
{
  SwitchGraphicsPipe *switch_pipe;
  DCAST_INTO_V(switch_pipe, _pipe);

  _egl_display = switch_pipe->_egl_display;
  _egl_surface = 0;

  PT(GraphicsWindowInputDevice) device = GraphicsWindowInputDevice::pointer_and_keyboard(this, "keyboard_mouse");
  add_input_device(device);
  _input = device;
}

/**
 *
 */
SwitchGraphicsWindow::
~SwitchGraphicsWindow() {
  destroy_surface();
}

/**
 * This function will be called within the draw thread before beginning
 * rendering for a given frame.  It should do whatever setup is required, and
 * return true if the frame should be rendered, or false if it should be
 * skipped.
 */
bool SwitchGraphicsWindow::
begin_frame(FrameMode mode, Thread *current_thread) {
  PStatTimer timer(_make_current_pcollector, current_thread);

  begin_frame_spam(mode);
  if (_gsg == nullptr) {
    return false;
  }

  // XXX not open yet.
  if (_egl_surface == EGL_NO_SURFACE) {
    return false;
  }

  SwitchGraphicsStateGuardian *switchgsg;
  DCAST_INTO_R(switchgsg, _gsg, false);
  {
    if (eglGetCurrentDisplay() == _egl_display &&
        eglGetCurrentSurface(EGL_READ) == _egl_surface &&
        eglGetCurrentSurface(EGL_DRAW) == _egl_surface &&
        eglGetCurrentContext() == switchgsg->_context) {
      // No need to make the context current again.  Short-circuit this
      // possibly-expensive call.
    } else {
      // Need to set the context.
      if (!eglMakeCurrent(_egl_display, _egl_surface, _egl_surface, switchgsg->_context)) {
        switchdisplay_cat.error() << "Failed to call eglMakeCurrent: "
          << get_egl_error_string(eglGetError()) << "\n";
      }
    }
  }

  // Now that we have made the context current to a window, we can reset the
  // GSG state if this is the first time it has been used.  (We can't just
  // call reset() when we construct the GSG, because reset() requires having a
  // current context.)
  switchgsg->reset_if_new();

  if (mode == FM_render) {
    // begin_render_texture();
    clear_cube_map_selection();
  }

  _gsg->set_current_properties(&get_fb_properties());
  return _gsg->begin_frame(current_thread);
}

/**
 * This function will be called within the draw thread after rendering is
 * completed for a given frame.  It should do whatever finalization is
 * required.
 */
void SwitchGraphicsWindow::
end_frame(FrameMode mode, Thread *current_thread) {
  end_frame_spam(mode);
  nassertv(_gsg != nullptr);

  if (mode == FM_render) {
    // end_render_texture();
    copy_to_textures();
  }

  _gsg->end_frame(current_thread);

  if (mode == FM_render) {
    trigger_flip();
    clear_cube_map_selection();
  }
}

/**
 * This function will be called within the draw thread after begin_flip() has
 * been called on all windows, to finish the exchange of the front and back
 * buffers.
 *
 * This should cause the window to wait for the flip, if necessary.
 */
void SwitchGraphicsWindow::
end_flip() {
  if (_gsg != nullptr && _flip_ready) {

    // It doesn't appear to be necessary to ensure the graphics context is
    // current before flipping the windows, and insisting on doing so can be a
    // significant performance hit.

    // make_current();

    if (_egl_surface != EGL_NO_SURFACE) {
      eglSwapBuffers(_egl_display, _egl_surface);
    }
  }
  GraphicsWindow::end_flip();
}

/**
 * Do whatever processing is necessary to ensure that the window responds to
 * user events.  Also, honor any requests recently made via
 * request_properties()
 *
 * This function is called only within the window thread.
 */
void SwitchGraphicsWindow::
process_events() {
  GraphicsWindow::process_events();

  // Loop until all events are read and processed.
  bool res = switch_process_events();
  if (!res) {
    switchdisplay_cat.error() << "switch_process_events(): time to exit.\n";
  }
}

/**
 * Applies the requested set of properties to the window, if possible, for
 * instance to request a change in size or minimization status.
 *
 * The window properties are applied immediately, rather than waiting until
 * the next frame.  This implies that this method may *only* be called from
 * within the window thread.
 *
 * The return value is true if the properties are set, false if they are
 * ignored.  This is mainly useful for derived classes to implement extensions
 * to this function.
 */
void SwitchGraphicsWindow::
set_properties_now(WindowProperties &properties) {
  if (_pipe == nullptr) {
    // If the pipe is null, we're probably closing down.
    GraphicsWindow::set_properties_now(properties);
    return;
  }

  GraphicsWindow::set_properties_now(properties);
  if (!properties.is_any_specified()) {
    // The base class has already handled this case.
    return;
  }

  /* TODO what can we actually change? */
}

/**
 * Closes the window right now.  Called from the window thread.
 */
void SwitchGraphicsWindow::
close_window() {
  destroy_surface();

  if (_gsg != nullptr) {
    _gsg.clear();
  }

  GraphicsWindow::close_window();

  // TODO null out Switch callbacks
}

/**
 * Opens the window right now.  Called from the window thread.  Returns true
 * if the window is successfully opened, or false if there was a problem.
 */
bool SwitchGraphicsWindow::
open_window() {
  // GSG CreationInitialization
  SwitchGraphicsStateGuardian *switchgsg;
  if (_gsg == 0) {
    // There is no old gsg.  Create a new one.
    switchgsg = new SwitchGraphicsStateGuardian(_engine, _pipe, nullptr);
    switchgsg->choose_pixel_format(_fb_properties, false, false);
    _gsg = switchgsg;
  } else {
    // If the old gsg has the wrong pixel format, create a new one that shares
    // with the old gsg.
    DCAST_INTO_R(switchgsg, _gsg, false);
    if (!switchgsg->get_fb_properties().subsumes(_fb_properties)) {
      switchgsg = new SwitchGraphicsStateGuardian(_engine, _pipe, switchgsg);
      switchgsg->choose_pixel_format(_fb_properties, false, false);
      _gsg = switchgsg;
    }
  }

  create_surface();

  // create_surface should have been called by now.
  if (_egl_surface == EGL_NO_SURFACE) {
    return false;
  }

  // Set some other properties.
  _properties.set_origin(0, 0);
  _properties.set_cursor_hidden(true);
  _properties.set_undecorated(true);

  if (!switchgsg->get_fb_properties().verify_hardware_software
      (_fb_properties, switchgsg->get_gl_renderer())) {
    close_window();
    return false;
  }

  _fb_properties = switchgsg->get_fb_properties();

  return true;
}

/**
 * Terminates the EGL surface.
 */
void SwitchGraphicsWindow::
destroy_surface() {
  if (_egl_surface != EGL_NO_SURFACE) {
    if (!eglDestroySurface(_egl_display, _egl_surface)) {
      switchdisplay_cat.error() << "Failed to destroy surface: "
        << get_egl_error_string(eglGetError()) << "\n";
    }
    _egl_surface = EGL_NO_SURFACE;
  }

  // Destroy the current context.
  if (_gsg != nullptr) {
    SwitchGraphicsStateGuardian *switchgsg;
    DCAST_INTO_V(switchgsg, _gsg);
    switchgsg->destroy_context();
  }
}

/**
 * Creates the EGL surface.
 */
bool SwitchGraphicsWindow::
create_surface() {
  SwitchGraphicsStateGuardian *switchgsg;
  DCAST_INTO_R(switchgsg, _gsg, false);

  NWindow *switch_window = switch_get_native_window();

  // Create the EGL surface.
  _egl_surface = eglCreateWindowSurface(_egl_display, switchgsg->_fbconfig, switch_window, nullptr);
  if (eglGetError() != EGL_SUCCESS) {
    switchdisplay_cat.error()
      << "Failed to create window surface.\n";
    return false;
  }

  // Create a context.
  if (switchgsg->_context == EGL_NO_CONTEXT) {
    if (!switchgsg->create_context()) {
      return false;
    }
  }

  // Switch to our newly created context.
  if (!eglMakeCurrent(_egl_display, _egl_surface, _egl_surface, switchgsg->_context)) {
    switchdisplay_cat.error() << "Failed to call eglMakeCurrent: "
      << get_egl_error_string(eglGetError()) << "\n";
  }

  // Query the size of the surface.  EGLint width, height;
  // eglQuerySurface(_egl_display, _egl_surface, EGL_WIDTH, &width);
  // eglQuerySurface(_egl_display, _egl_surface, EGL_HEIGHT, &height);

  switchgsg->reset_if_new();
  if (!switchgsg->is_valid()) {
    close_window();
    return false;
  }

  return true;
}

/* TODO handle input */