#include <assert.h>

#include "sc_layer.h"
#include "wl.h"

static void
xdg_surface_configure(void *data, struct xdg_surface *xdg_surface,
					  uint32_t serial)
{
	LOG("xdg_surface_ack_configure\n");
//	struct wui_layer *layer = (struct wui_layer *) data;
	xdg_surface_ack_configure(xdg_surface, serial);
}

struct sc_animation_v1 *animation;
const struct xdg_surface_listener xdg_surface_listener = {
	.configure = xdg_surface_configure,
};

struct wui_layer *
layer_create(int width, int height)
{

	struct wayland_config *wlconfig = wui_wl_context();

	struct wui_layer *layer =
		(struct wui_layer *) calloc(1, sizeof(struct wui_layer));
	
	view_init(&layer->base, width, height);
	view_init_buffer(&layer->base);
	//wl_surface_set_user_data(window->base.surface, window);

    assert(wlconfig->sc_layer_shell);
    layer->layer = sc_shell_unstable_v1_get_layer_surface(wlconfig->sc_layer_shell, layer->base.surface, NULL);
	sc_layer_surface_v1_set_bounds(layer->layer, 
		wl_fixed_from_double(0),
		wl_fixed_from_double(0),
		wl_fixed_from_double(200),
		wl_fixed_from_double(200));
	sc_layer_surface_v1_set_position(layer->layer,
		wl_fixed_from_double(50),
		wl_fixed_from_double(50));
	sc_layer_surface_v1_set_background_color(layer->layer,
		255, 0, 0, 255);
	sc_layer_surface_v1_set_border_color(layer->layer, 0, 0, 0, 255);
	sc_layer_surface_v1_set_border_width(layer->layer, wl_fixed_from_double(4));
	sc_layer_surface_v1_set_corner_radius(layer->layer, wl_fixed_from_double(20));


	layer->region = wl_compositor_create_region(wlconfig->wl_compositor);

	wl_region_add(layer->region, 0, 0, layer->base.width, layer->base.height);
	wl_surface_set_opaque_region(layer->base.surface, layer->region);

	animation = sc_shell_unstable_v1_get_animation(
		wlconfig->sc_layer_shell,
		wl_fixed_from_double(2.0), // duration
		wl_fixed_from_double(1.0), // speed
		1, //repeat-count
		0 // autoreverse
	);

	wl_surface_commit(layer->base.surface);

	wl_display_dispatch_pending(wlconfig->wl_display);
	return layer;
}
