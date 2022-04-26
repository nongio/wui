#ifndef _WUI_SKIA_H
#define _WUI_SKIA_H

#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>

#define SK_GL
// #define SK_ASSUME_GL_ES
#include <include/gpu/GrDirectContext.h>
#include <include/gpu/gl/GrGLInterface.h>
#include <include/core/SkSurface.h>
#include <include/core/SkCanvas.h>
#include <include/gpu/gl/GrGLTypes.h>


#include "view.h"

struct skia_context {
    sk_sp<GrDirectContext> context;
    sk_sp<SkSurface> surface;
};

struct skia_context *skia_context_create_for_view(struct wui_view *view);
#endif