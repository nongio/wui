#include "skia.h"
#include "egl.h"
#include "wl.h"


struct skia_context *skia_context_create_for_window(struct wui_window *win)
{

    struct skia_context *skia = (struct skia_context *)calloc(1, sizeof(struct skia_context));

    auto gl = GrGLMakeNativeInterface();
    skia->context = GrDirectContext::MakeGL(gl);

    SkImageInfo info = SkImageInfo:: MakeN32Premul(win->width * win->content_scale,
                                    win->height * win->content_scale);

    GrGLint buffer;
    glGetIntegerv(GL_FRAMEBUFFER_BINDING, &buffer);

    GrGLFramebufferInfo fbInfo;
    fbInfo.fFBOID = buffer;
    fbInfo.fFormat = GL_RGBA8_OES;

    GrBackendRenderTarget backendRT(win->width * win->content_scale,
                                    win->height * win->content_scale,
                                    1,
                                    8,
                                    fbInfo);

    skia->surface = SkSurface::MakeFromBackendRenderTarget(skia->context.get(), backendRT,
                                                       kBottomLeft_GrSurfaceOrigin,
                                                       kRGBA_8888_SkColorType,
                                                       nullptr,
                                                       nullptr);
    
    if (!skia->surface) {
        SkDebugf("SkSurface::MakeRenderTarget returned null\n");
        exit(1);
    }    
    return skia;
}