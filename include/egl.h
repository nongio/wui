#ifndef _WUI_EGL_H
#define _WUI_EGL_H

#include <wayland-egl.h> // Wayland EGL MUST be included before EGL headers
#include <GLES2/gl2.h>
#include <EGL/egl.h>
#include <EGL/eglplatform.h>

#include <GLES2/gl2.h>


struct egl_context {
  /// Native System informations
  EGLNativeDisplayType native_display;
  EGLNativeWindowType native_window;
  /// EGL display
  EGLDisplay  display;
  /// EGL context
  EGLContext  context;
  /// EGL surface
  EGLSurface  surface;
};

struct egl_context *create_egl_context(EGLNativeDisplayType display_id,
                                       EGLNativeWindowType native_window);
#endif