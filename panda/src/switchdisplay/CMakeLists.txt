if(NOT SWITCH)
  return()
endif()

if(NOT HAVE_GLES1 AND NOT HAVE_GLES2)
  return()
endif()

set(P3SWITCHDISPLAY_HEADERS
  config_switchdisplay.h
  switchGraphicsPipe.h switchGraphicsPipe.I
  switchGraphicsStateGuardian.h switchGraphicsStateGuardian.I
  switchGraphicsWindow.h switchGraphicsWindow.I
  libnxWrapper.h
)

set(P3SWITCHDISPLAY_SOURCES
  config_switchdisplay.cxx
  switchGraphicsPipe.cxx
  switchGraphicsStateGuardian.cxx
  switchGraphicsWindow.cxx
  libnxWrapper.cxx
)

composite_sources(p3switchdisplay P3SWITCHDISPLAY_SOURCES)

# We actually build this twice!  Once for GLES1, once for GLES2.
if(HAVE_GLES1)
  add_component_library(p3switchdisplay_gles1 SYMBOL BUILDING_PANDAGLES
    INIT init_libswitchdisplay config_switchdisplay.h
    ${P3SWITCHDISPLAY_HEADERS} ${P3SWITCHDISPLAY_SOURCES})
  target_compile_definitions(p3switchdisplay_gles1 PUBLIC OPENGLES_1)
  target_link_libraries(p3switchdisplay_gles1 p3glesgsg p3x11display
    PKG::EGL PKG::GLES1)

  if(NOT BUILD_METALIBS)
    install(TARGETS p3switchdisplay_gles1
      EXPORT OpenGLES1 COMPONENT OpenGLES1
      DESTINATION ${CMAKE_INSTALL_LIBDIR}
      RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
      INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/panda3d
      ARCHIVE COMPONENT OpenGLES1Devel)
  endif()

  install(FILES ${P3SWITCHDISPLAY_HEADERS} COMPONENT OpenGLES1Devel DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/panda3d)
endif()

if(HAVE_GLES2)
  add_component_library(p3switchdisplay_gles2 SYMBOL BUILDING_PANDAGLES2
    INIT init_libswitchdisplay config_switchdisplay.h
    ${P3SWITCHDISPLAY_HEADERS} ${P3SWITCHDISPLAY_SOURCES})
  target_compile_definitions(p3switchdisplay_gles2 PUBLIC OPENGLES_2)
  target_link_libraries(p3switchdisplay_gles2 p3gles2gsg p3x11display
    PKG::EGL PKG::GLES2)

  if(NOT BUILD_METALIBS)
    install(TARGETS p3switchdisplay_gles2
      EXPORT OpenGLES2 COMPONENT OpenGLES2
      DESTINATION ${CMAKE_INSTALL_LIBDIR}
      RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
      INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/panda3d
      ARCHIVE COMPONENT OpenGLES2Devel)
  endif()

  install(FILES ${P3SWITCHDISPLAY_HEADERS} COMPONENT OpenGLES2Devel DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/panda3d)
endif()
