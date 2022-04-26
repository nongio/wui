
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>

#include <include/core/SkFont.h>
#include <include/core/SkFontMgr.h>
#include <include/core/SkRRect.h>
#include <include/core/SkRegion.h>
#include <include/core/SkBlurTypes.h>
#include <include/effects/SkBlurMaskFilter.h>
#include <Yoga.h>

#include "log.h"
#include "skia.h"
#include "view.h"
#include "sc_layer.h"
#include "wl.h"

void
draw(SkCanvas *canvas)
{
	LOG("draw\n");
	SkMatrix matrix;
	matrix.setScale(2.0f, 2.0f);
	canvas->setMatrix(matrix);

	canvas->clear(SK_ColorTRANSPARENT);

	SkPaint p;
	p.setStrokeWidth(1);
	p.setColor(0xFFf6f6f6);
	p.setAntiAlias(true);
	p.setStyle(SkPaint::kFill_Style);

	SkRect rect = SkRect::MakeXYWH(0, 0, 500, 500);

	canvas->drawRect(rect, p);

	SkPaint paint;
	SkFont font;
	//sk_sp<SkFontMgr> mgr(SkFontMgr::RefDefault());
	sk_sp<SkTypeface> tf = SkTypeface::MakeFromName("TeX Gyre Heros", SkFontStyle::Bold());
	paint.setStyle(SkPaint::kFill_Style);
	paint.setColor(0xFF000000);
	font.setTypeface(tf);

	float textSizes[] = {12, 18, 24, 36};
	for (auto size : textSizes) {
		font.setSize(size * 2);
		canvas->drawString("Layers animations", 10, 20, font, paint);
		canvas->translate(0, size * 3);
	}

}

int
main()
{
	LOG("Hello world...\n");
	struct wayland_config *wl = wui_wl_init();
	
	int width = 500;
	int height = 500;
	struct wui_layer *layer = layer_create(width, height);
	
	wl_display_roundtrip(wl->wl_display);

	draw(layer->base.skia->surface->getCanvas());
	layer->base.skia->context->flushAndSubmit();
	eglSwapBuffers(layer->base.egl->display, layer->base.egl->surface);

	float radius = 0;
	float add = 1.0;
	while (wl_display_dispatch(wl->wl_display) != -1) {
		LOG("dispatch\n");
		radius += add;
		if(radius >= 100.0) {
			add = -1.0;
		}
		if(radius <= 0) {
			add = 1.0;
		}
		sc_layer_surface_v1_set_corner_radius(layer->layer, wl_fixed_from_double(radius));
		wl_surface_commit(layer->base.surface);
	}

	wl_display_disconnect(wl->wl_display);

	exit(0);
}

