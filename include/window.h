#ifndef _WUI_WINDOW_H
#define _WUI_WINDOW_H

#include <wayland-client-core.h>
#include <wayland-client-protocol.h>
#include <wayland-client.h>

#include "egl.h"
#include "skia.h"
#include "wl.h"
#include "view.h"
#include "xdg-decoration-unstable-v1-protocol.h"

struct wui_window {
	struct wui_view base;
	
	struct xdg_surface *xdg_surface;
	struct xdg_toplevel *xdg_toplevel;
	struct zxdg_toplevel_decoration_v1 *decoration;
	struct wl_region *region;
};

struct wui_window *window_create(int width, int height);

#endif