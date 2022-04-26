#include "window.h"
#include "wl.h"

bool mode = false;

static void
xdg_toplevel_handle_configure(void *data, struct xdg_toplevel *xdg_toplevel,
							  int32_t w, int32_t h, struct wl_array *states)
{

	LOG("xdg_toplevel_handle_configure !\n");
	struct wui_window *window = (struct wui_window *) data;

	// no window geometry event, ignore
	if (w == 0 && h == 0)
		return;

	if (window->base.wl->decoration_manager && mode == false) {
		mode = true;
		// Let the compositor do all the complicated window management
		window->decoration =
			zxdg_decoration_manager_v1_get_toplevel_decoration(
				window->base.wl->decoration_manager, window->xdg_toplevel);
		zxdg_toplevel_decoration_v1_set_mode(window->decoration,
			ZXDG_TOPLEVEL_DECORATION_V1_MODE_SERVER_SIDE);
	}
	// window resized
	if (window->base.width != w && window->base.height != h) {
		window->base.width = w;
		window->base.height = h;

		wl_egl_window_resize(window->base.egl->native_window, w, h, 0, 0);
		wl_surface_commit(window->base.surface);
	}
}

static void
xdg_toplevel_handle_close(void *data, struct xdg_toplevel *xdg_toplevel)
{
	LOG("close\n");
	struct wui_window *window = (struct wui_window *) data;
	wl_display_disconnect(window->base.wl->wl_display);

	// window closed, be sure that this event gets processed
	// program_alive = false;
}

struct xdg_toplevel_listener xdg_toplevel_listener = {
	.configure = xdg_toplevel_handle_configure,
	.close = xdg_toplevel_handle_close,
};



static void
xdg_surface_configure(void *data, struct xdg_surface *xdg_surface,
					  uint32_t serial)
{
	LOG("xdg_surface_ack_configure\n");
	struct wui_window *window = (struct wui_window *) data;
	xdg_surface_ack_configure(xdg_surface, serial);
}

const struct xdg_surface_listener xdg_surface_listener = {
	.configure = xdg_surface_configure,
};

struct wui_window *
window_create(int width, int height)
{

	struct wayland_config *wlconfig = wui_wl_context();

	struct wui_window *window =
		(struct wui_window *) calloc(1, sizeof(struct wui_window));
	
	view_init(&window->base, width, height);
	view_init_buffer(&window->base);
	//wl_surface_set_user_data(window->base.surface, window);

	
	window->xdg_surface =
		xdg_wm_base_get_xdg_surface(wlconfig->xdg_wm_base, window->base.surface);
	xdg_surface_add_listener(window->xdg_surface, &xdg_surface_listener, window);

	window->xdg_toplevel = xdg_surface_get_toplevel(window->xdg_surface);
	xdg_toplevel_set_title(window->xdg_toplevel, "Wayland EGL example");
	xdg_toplevel_add_listener(window->xdg_toplevel, &xdg_toplevel_listener,
							  window);

	// create native window
	window->region = wl_compositor_create_region(wlconfig->wl_compositor);

	wl_region_add(window->region, 0, 0, window->base.width, window->base.height);
	wl_surface_set_opaque_region(window->base.surface, window->region);

	

	wl_surface_commit(window->base.surface);

	wl_display_dispatch_pending(wlconfig->wl_display);
	return window;
}