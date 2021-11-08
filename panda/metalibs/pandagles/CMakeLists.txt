if(NOT HAVE_GLES1 OR NOT HAVE_EGL)
  return()
endif()

set(PANDAGLES_LINK_TARGETS p3glesgsg p3glstuff)

if(ANDROID)
  set(GLES1_PIPE_TYPE "AndroidGraphicsPipe")
  set(GLES1_PIPE_INCLUDE "androidGraphicsPipe.h")

elseif(SWITCH)
  set(GLES1_PIPE_TYPE "SwitchGraphicsPipe")
  set(GLES1_PIPE_INCLUDE "switchGraphicsPipe.h")
  list(APPEND PANDAGLES_LINK_TARGETS p3switchdisplay_gles1)

else()
  set(GLES1_PIPE_TYPE "eglGraphicsPipe")
  set(GLES1_PIPE_INCLUDE "eglGraphicsPipe.h")
  list(APPEND PANDAGLES_LINK_TARGETS p3egldisplay_gles1)

endif()

set(CMAKE_INSTALL_DEFAULT_COMPONENT_NAME "OpenGLES1Devel")
add_metalib(pandagles ${MODULE_TYPE}
  INCLUDE "${GLES1_PIPE_INCLUDE}"
  INIT init_libpandagles pandagles.h
  EXPORT int get_pipe_type_pandagles "${GLES1_PIPE_TYPE}::get_class_type().get_index()"
  COMPONENTS ${PANDAGLES_LINK_TARGETS})
unset(CMAKE_INSTALL_DEFAULT_COMPONENT_NAME)

if(HAVE_X11)
  target_compile_definitions(pandagles PUBLIC USE_X11)
else()
  target_compile_definitions(pandagles PRIVATE EGL_NO_X11)
endif()

install(TARGETS pandagles
  EXPORT OpenGLES1 COMPONENT OpenGLES1
  DESTINATION ${MODULE_DESTINATION}
  ARCHIVE COMPONENT OpenGLES1Devel)

export_targets(OpenGLES1 COMPONENT OpenGLES1Devel)
