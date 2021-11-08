/**
 * PANDA 3D SOFTWARE
 * Copyright (c) Carnegie Mellon University.  All rights reserved.
 *
 * All use of this software is subject to the terms of the revised BSD
 * license.  You should have received a copy of this license along
 * with this source code in a file named "LICENSE."
 *
 * @file switchGraphicsWindow.h
 * @author rdb
 * @date 2013-01-11
 */

#ifndef SWITCHGRAPHICSWINDOW_H
#define SWITCHGRAPHICSWINDOW_H

#include "pandabase.h"

#include "switchGraphicsPipe.h"
#include "graphicsWindow.h"
#include "buttonHandle.h"

/**
 * An interface to manage Switch windows and their appropriate EGL surfaces.
 */
class SwitchGraphicsWindow : public GraphicsWindow {
public:
  SwitchGraphicsWindow(GraphicsEngine *engine, GraphicsPipe *pipe,
                        const std::string &name,
                        const FrameBufferProperties &fb_prop,
                        const WindowProperties &win_prop,
                        int flags,
                        GraphicsStateGuardian *gsg,
                        GraphicsOutput *host);
  virtual ~SwitchGraphicsWindow();

  virtual bool begin_frame(FrameMode mode, Thread *current_thread);
  virtual void end_frame(FrameMode mode, Thread *current_thread);
  virtual void end_flip();

  virtual void process_events();
  virtual void set_properties_now(WindowProperties &properties);

protected:
  virtual void close_window();
  virtual bool open_window();

  virtual void destroy_surface();
  virtual bool create_surface();

private:
  EGLDisplay _egl_display;
  EGLSurface _egl_surface;

  int32_t _mouse_button_state;

  GraphicsWindowInputDevice *_input;

public:
  static TypeHandle get_class_type() {
    return _type_handle;
  }
  static void init_type() {
    GraphicsWindow::init_type();
    register_type(_type_handle, "SwitchGraphicsWindow",
                  GraphicsWindow::get_class_type());
  }
  virtual TypeHandle get_type() const {
    return get_class_type();
  }
  virtual TypeHandle force_init_type() {init_type(); return get_class_type();}

private:
  static TypeHandle _type_handle;
};

#include "switchGraphicsWindow.I"

#endif
