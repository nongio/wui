#ifndef _WUI_VIEW_H
#define _WUI_VIEW_H

#include <wayland-client-core.h>
#include <wayland-client-protocol.h>
#include <wayland-client.h>

#include "egl.h"
#include "skia.h"
#include "wl.h"

struct wui_view {
	struct egl_context *egl;
	struct wayland_config *wl;

	struct wl_surface *surface;
    
	struct xdg_surface *xdg_surface;

	int width;
	int height;
	double content_scale;

	struct wl_egl_window *egl_window;
	struct wl_region *region;

	struct skia_context *skia;
};

struct wui_subview {
    struct wui_view base;
    struct wl_subsurface *subsurface;
};

struct wui_view *view_create(int width, int height);
struct wui_subview *subview_create(struct wl_surface*parent, int x, int y, int width, int height);
void view_init(struct wui_view *view, int w, int h);
void view_init_buffer(struct wui_view *view);

#endif