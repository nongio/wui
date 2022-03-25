#ifndef _WUI_WINDOW_H
#define _WUI_WINDOW_H

#include <wayland-client-core.h>
#include <wayland-client-protocol.h>
#include <wayland-client.h>

#include "egl.h"
#include "skia.h"

struct wui_window {
	struct egl_context *egl;

	struct wl_surface *surface;
	struct xdg_surface *xdg_surface;
	struct xdg_toplevel *xdg_toplevel;

	int width;
	int height;
	double content_scale;
	struct wl_egl_window *egl_window;
	struct wl_region *region;

	struct skia_context *skia;
};

struct wui_window *window_create(int width, int height);

#endif