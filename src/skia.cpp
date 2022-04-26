#include "skia.h"
#include "egl.h"
#include "wl.h"


struct skia_context *skia_context_create_for_view(struct wui_view *view)
{

    struct skia_context *skia = (struct skia_context *)calloc(1, sizeof(struct skia_context));

    auto gl = GrGLMakeNativeInterface();
    gl->ref();
    skia->context = GrDirectContext::MakeGL(gl);
    skia->context->ref();

    SkImageInfo info = SkImageInfo:: MakeN32Premul(view->width * view->content_scale,
                                    view->height * view->content_scale);

    GrGLint buffer;
    glGetIntegerv(GL_FRAMEBUFFER_BINDING, &buffer);

    GrGLFramebufferInfo fbInfo;
    fbInfo.fFBOID = buffer;
    fbInfo.fFormat = GL_RGBA8_OES;

    GrBackendRenderTarget backendRT(view->width * view->content_scale,
                                    view->height * view->content_scale,
                                    1,
                                    8,
                                    fbInfo);

    skia->surface = SkSurface::MakeFromBackendRenderTarget(skia->context.get(), backendRT,
                                                       kBottomLeft_GrSurfaceOrigin,
                                                       kRGBA_8888_SkColorType,
                                                       nullptr,
                                                       nullptr);
    skia->surface->ref();
    if (!skia->surface) {
        SkDebugf("SkSurface::MakeRenderTarget returned null\n");
        exit(1);
    }    
    return skia;
}