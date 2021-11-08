if(NOT HAVE_GLES2 OR NOT HAVE_EGL)
  return()
endif()

set(PANDAGLES2_LINK_TARGETS p3gles2gsg p3glstuff)

if(ANDROID)
  set(GLES2_PIPE_TYPE "AndroidGraphicsPipe")
  set(GLES2_PIPE_INCLUDE "androidGraphicsPipe.h")

elseif(SWITCH)
  set(GLES2_PIPE_TYPE "SwitchGraphicsPipe")
  set(GLES2_PIPE_INCLUDE "switchGraphicsPipe.h")
  list(APPEND PANDAGLES2_LINK_TARGETS p3switchdisplay_gles2)

else()
  set(GLES2_PIPE_TYPE "eglGraphicsPipe")
  set(GLES2_PIPE_INCLUDE "eglGraphicsPipe.h")
  list(APPEND PANDAGLES2_LINK_TARGETS p3egldisplay_gles2)

endif()


set(CMAKE_INSTALL_DEFAULT_COMPONENT_NAME "OpenGLES2Devel")
add_metalib(pandagles2 ${MODULE_TYPE}
  INCLUDE ${GLES2_PIPE_INCLUDE}
  INIT init_libpandagles2 pandagles2.h
  EXPORT int get_pipe_type_pandagles2 "${GLES2_PIPE_TYPE}::get_class_type().get_index()"
  COMPONENTS ${PANDAGLES2_LINK_TARGETS})
unset(CMAKE_INSTALL_DEFAULT_COMPONENT_NAME)

if(HAVE_X11)
  target_compile_definitions(pandagles2 PUBLIC USE_X11)
else()
  target_compile_definitions(pandagles2 PRIVATE EGL_NO_X11)
endif()

install(TARGETS pandagles2
  EXPORT OpenGLES2 COMPONENT OpenGLES2
  DESTINATION ${MODULE_DESTINATION}
  ARCHIVE COMPONENT OpenGLES2Devel)

export_targets(OpenGLES2 COMPONENT OpenGLES2Devel)
