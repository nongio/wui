#ifndef _WUI_SC_LAYER_H
#define _WUI_SC_LAYER_H

#include <wayland-client-core.h>
#include <wayland-client-protocol.h>
#include <wayland-client.h>

#include "sc-layer-unstable-v1-protocol.h"

#include "egl.h"
#include "skia.h"
#include "wl.h"
#include "view.h"

struct wui_layer {
	struct wui_view base;
	
	struct xdg_surface *xdg_surface;
	struct wl_region *region;

	struct sc_layer_surface_v1 *layer;
};

struct wui_layer *layer_create(int width, int height);

#endif