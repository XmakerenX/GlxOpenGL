# Try to find Wayland on a Unix system
#
# This will define:
#
#   WAYLAND_FOUND       - True if Wayland is found
#   WAYLAND_LIBRARIES   - Link these to use Wayland
#   WAYLAND_INCLUDE_DIR - Include directory for Wayland
#   WAYLAND_DEFINITIONS - Compiler flags for using Wayland
#
# In addition the following more fine grained variables will be defined:
#
#   WAYLAND_CLIENT_FOUND  WAYLAND_CLIENT_INCLUDE_DIR  WAYLAND_CLIENT_LIBRARIES
#   WAYLAND_SERVER_FOUND  WAYLAND_SERVER_INCLUDE_DIR  WAYLAND_SERVER_LIBRARIES
#   WAYLAND_EGL_FOUND     WAYLAND_EGL_INCLUDE_DIR     WAYLAND_EGL_LIBRARIES
#
# Copyright (c) 2013 Martin Gräßlin <mgraesslin@kde.org>
#
# Redistribution and use is allowed according to the terms of the BSD license.
# For details see the accompanying COPYING-CMAKE-SCRIPTS file.

IF (NOT WIN32)
  IF (WAYLAND_INCLUDE_DIR AND WAYLAND_LIBRARIES)
    # In the cache already
    SET(WAYLAND_FIND_QUIETLY TRUE)
  ENDIF ()

  # Use pkg-config to get the directories and then use these values
  # in the FIND_PATH() and FIND_LIBRARY() calls
  FIND_PACKAGE(PkgConfig)
  PKG_CHECK_MODULES(PKG_WAYLAND QUIET wayland-client wayland-server wayland-egl wayland-cursor)

  SET(WAYLAND_DEFINITIONS ${PKG_WAYLAND_CFLAGS})

  FIND_PATH(WAYLAND_CLIENT_INCLUDE_DIR  NAMES wayland-client.h HINTS ${PKG_WAYLAND_INCLUDE_DIRS})
  FIND_PATH(WAYLAND_SERVER_INCLUDE_DIR  NAMES wayland-server.h HINTS ${PKG_WAYLAND_INCLUDE_DIRS})
  FIND_PATH(WAYLAND_EGL_INCLUDE_DIR     NAMES wayland-egl.h    HINTS ${PKG_WAYLAND_INCLUDE_DIRS})
  FIND_PATH(WAYLAND_CURSOR_INCLUDE_DIR  NAMES wayland-cursor.h HINTS ${PKG_WAYLAND_INCLUDE_DIRS})

  FIND_LIBRARY(WAYLAND_CLIENT_LIBRARIES NAMES wayland-client   HINTS ${PKG_WAYLAND_LIBRARY_DIRS})
  FIND_LIBRARY(WAYLAND_SERVER_LIBRARIES NAMES wayland-server   HINTS ${PKG_WAYLAND_LIBRARY_DIRS})
  FIND_LIBRARY(WAYLAND_EGL_LIBRARIES    NAMES wayland-egl      HINTS ${PKG_WAYLAND_LIBRARY_DIRS})
  FIND_LIBRARY(WAYLAND_CURSOR_LIBRARIES NAMES wayland-cursor   HINTS ${PKG_WAYLAND_LIBRARY_DIRS})

  set(WAYLAND_INCLUDE_DIR ${WAYLAND_CLIENT_INCLUDE_DIR} ${WAYLAND_SERVER_INCLUDE_DIR} ${WAYLAND_EGL_INCLUDE_DIR} ${WAYLAND_CURSOR_INCLUDE_DIR})

  set(WAYLAND_LIBRARIES ${WAYLAND_CLIENT_LIBRARIES} ${WAYLAND_SERVER_LIBRARIES} ${WAYLAND_EGL_LIBRARIES} ${WAYLAND_CURSOR_LIBRARIES})

  list(REMOVE_DUPLICATES WAYLAND_INCLUDE_DIR)

  include(FindPackageHandleStandardArgs)

  FIND_PACKAGE_HANDLE_STANDARD_ARGS(WAYLAND_CLIENT  DEFAULT_MSG  WAYLAND_CLIENT_LIBRARIES  WAYLAND_CLIENT_INCLUDE_DIR)
  FIND_PACKAGE_HANDLE_STANDARD_ARGS(WAYLAND_SERVER  DEFAULT_MSG  WAYLAND_SERVER_LIBRARIES  WAYLAND_SERVER_INCLUDE_DIR)
  FIND_PACKAGE_HANDLE_STANDARD_ARGS(WAYLAND_EGL     DEFAULT_MSG  WAYLAND_EGL_LIBRARIES     WAYLAND_EGL_INCLUDE_DIR)
  FIND_PACKAGE_HANDLE_STANDARD_ARGS(WAYLAND_CURSOR  DEFAULT_MSG  WAYLAND_CURSOR_LIBRARIES  WAYLAND_CURSOR_INCLUDE_DIR)
  FIND_PACKAGE_HANDLE_STANDARD_ARGS(WAYLAND         DEFAULT_MSG  WAYLAND_LIBRARIES         WAYLAND_INCLUDE_DIR)

  MARK_AS_ADVANCED(
        WAYLAND_INCLUDE_DIR         WAYLAND_LIBRARIES
        WAYLAND_CLIENT_INCLUDE_DIR  WAYLAND_CLIENT_LIBRARIES
        WAYLAND_SERVER_INCLUDE_DIR  WAYLAND_SERVER_LIBRARIES
        WAYLAND_EGL_INCLUDE_DIR     WAYLAND_EGL_LIBRARIES
        WAYLAND_CURSOR_INCLUDE_DIR  WAYLAND_CURSOR_LIBRARIES
  )

ENDIF ()
 
