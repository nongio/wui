#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>

#include "log.h"
#include "skia.h"
#include "window.h"
#include "wl.h"

void
draw(SkCanvas *canvas)
{
	SkMatrix matrix;
	matrix.setScale(2.0f, 2.0f);
	canvas->setMatrix(matrix);

	canvas->clear(SK_ColorTRANSPARENT);

	SkPaint p;
	p.setStrokeWidth(1);
	p.setColor(0xFF7d7d7d);
	p.setAntiAlias(true);
	p.setStyle(SkPaint::kFill_Style);

	SkRect rect = SkRect::MakeXYWH(0, 0, 500, 500);

	canvas->drawRoundRect(rect, 5, 5, p);

	SkPaint paint;
	SkFont font;
	sk_sp<SkFontMgr> mgr(SkFontMgr::RefDefault());
	uint32_t utf32string[] = {0x1F310};
	sk_sp<SkTypeface> tf(mgr->matchFamilyStyleCharacter(
		nullptr, SkFontStyle(), nullptr, 0, utf32string[0]));

	;
	paint.setStyle(SkPaint::kFill_Style);
	paint.setColor(0xFF000000);
	font.setTypeface(tf);

	float textSizes[] = {12, 18, 24, 36};
	for (auto size : textSizes) {
		font.setSize(size * 2);
		canvas->drawString("Aa  😀", 10, 20, font, paint);
		canvas->translate(0, size * 3);
	}

	p.setColor(0xFFFF0000);
	// canvas->drawCircle(50, 50, 50, p);
	canvas->drawCircle(0, 0, 2, p);
	canvas->drawCircle(100, 0, 2, p);
	canvas->drawCircle(0, 100, 2, p);
	canvas->drawCircle(100, 100, 2, p);
}

int
main()
{
	LOG("Hello world...\n");
	// struct WlContext *wl = wui_wl_init();
	int width = 500;
	int height = 500;
	struct wui_window *window = window_create(width, height);

	draw(window->skia->surface->getCanvas());

	window->skia->context->flushAndSubmit();
	eglSwapBuffers(window->egl->display, window->egl->surface);
	//
	while (wl_display_dispatch(window->wl->wl_display) != -1) {
    /* This space deliberately left blank */
	}

	wl_display_disconnect(window->wl->wl_display);

	exit(0);
}