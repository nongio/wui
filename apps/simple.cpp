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
	p.setColor(0xFFf6f6f6);
	p.setAntiAlias(true);
	p.setStyle(SkPaint::kFill_Style);

	SkRect rect = SkRect::MakeXYWH(0, 0, 500, 500);

	canvas->drawRoundRect(rect, 6, 6, p);

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

}

void
drawView(SkCanvas *canvas, SkColor color)
{
	SkMatrix matrix;
	matrix.setScale(2.0f, 2.0f);
	canvas->setMatrix(matrix);

	canvas->clear(SK_ColorTRANSPARENT);
    
    SkRRect rrect = SkRRect::MakeRectXY({10,10,160,60}, 10, 10);

    SkPaint p;
    p.setAntiAlias(true);
    p.setStyle(SkPaint::kFill_Style);
    p.setColor(SK_ColorWHITE);

    canvas->drawRRect(rrect, p);

    canvas->save();
	// canvas->clipRRect(rrect, )
    canvas->clipRRect(rrect , SkClipOp::kIntersect, true);

    p.setColor(SK_ColorBLACK);
    p.setStyle(SkPaint::kStroke_Style);
    p.setStrokeWidth(2);
    p.setMaskFilter(SkMaskFilter::MakeBlur(kNormal_SkBlurStyle, 2));

    SkRRect offset_rrect = SkRRect::MakeRectXY({10,11,160,61}, 10, 10);
    canvas->drawRRect(offset_rrect, p);

    canvas->restore();    

    p.setStrokeWidth(1);
    p.setMaskFilter(nullptr);
    p.setColor(SkColorSetARGB(255, 63, 127, 255));
    canvas->drawRRect(rrect, p);
}

void layout() {
	YGNodeRef root = YGNodeNew();
	YGNodeStyleSetWidth(root, 500);
	YGNodeStyleSetHeight(root, 500);
	YGNodeStyleSetFlexDirection(root, YGFlexDirectionColumn);
	YGNodeStyleSetJustifyContent(root, YGJustifyCenter);
	YGNodeStyleSetAlignItems(root, YGAlignCenter);

	YGNodeRef node = YGNodeNew();
	YGNodeStyleSetWidth(node, 100);
	YGNodeStyleSetHeight(node, 100);

	YGNodeInsertChild(root, node, 0);

	YGNodeCalculateLayout(root, YGUndefined, YGUndefined, YGDirectionLTR);

	LOG("layout: %f, %f\n", YGNodeLayoutGetLeft(node), YGNodeLayoutGetBottom(node));
}

int
main()
{
	LOG("Hello world...\n");
	struct wayland_config *wl = wui_wl_init();
	
	int width = 500;
	int height = 500;
	struct wui_window *window = window_create(width, height);
	
	wl_display_roundtrip(wl->wl_display);

	draw(window->base.skia->surface->getCanvas());
	window->base.skia->context->flushAndSubmit();
	eglSwapBuffers(window->base.egl->display, window->base.egl->surface);

	struct wui_subview *view = subview_create(window->base.surface, 10, 10, 200, 100);
	drawView(view->base.skia->surface->getCanvas(), SK_ColorBLUE);
	view->base.skia->context->flushAndSubmit();
	eglSwapBuffers(view->base.egl->display, view->base.egl->surface);

	struct wui_subview *view2 = subview_create(window->base.surface, 20, 200, 200, 100);

	drawView(view2->base.skia->surface->getCanvas(), SK_ColorRED);
	view2->base.skia->context->flushAndSubmit();	
	eglSwapBuffers(view2->base.egl->display, view2->base.egl->surface);

	layout();
	
	int x = 10;
	while (wl_display_dispatch(wl->wl_display) != -1) {
		LOG("loop\n");
	}

	wl_display_disconnect(wl->wl_display);

	exit(0);
}

