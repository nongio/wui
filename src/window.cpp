#include "window.h"
#include "wl.h"

static void
xdg_toplevel_handle_configure(void *data, struct xdg_toplevel *xdg_toplevel,
							  int32_t w, int32_t h, struct wl_array *states)
{

	LOG("xdg_toplevel_handle_configure !\n");
	struct wui_window *window = (struct wui_window *) data;

	// no window geometry event, ignore
	if (w == 0 && h == 0)
		return;

	// window resized
	if (window->width != w && window->height != h) {
		window->width = w;
		window->height = h;

		wl_egl_window_resize(window->egl->native_window, w, h, 0, 0);
		LOG("wl_surface_commit !\n");
		wl_surface_commit(window->surface);
	}
}

static void
xdg_toplevel_handle_close(void *data, struct xdg_toplevel *xdg_toplevel)
{
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
	xdg_surface_ack_configure(xdg_surface, serial);
}

const struct xdg_surface_listener xdg_surface_listener = {
	.configure = xdg_surface_configure,
};

struct wui_window *
window_create(int width, int height)
{

	struct wl_context *context = wui_wl_context();

	struct wui_window *window =
		(struct wui_window *) calloc(1, sizeof(struct wui_window));

	window->width = width;
	window->height = height;
	window->content_scale = 2;
	window->surface = wl_compositor_create_surface(context->wl_compositor);

	if (window->surface == NULL) {
		LOG("No Compositor surface ! Yay....\n");
		exit(1);
	} else {
		LOG("Got a compositor surface !\n");
	}
	window->xdg_surface =
		xdg_wm_base_get_xdg_surface(context->xdg_wm_base, window->surface);
	xdg_surface_add_listener(window->xdg_surface, &xdg_surface_listener, NULL);

	window->xdg_toplevel = xdg_surface_get_toplevel(window->xdg_surface);
	xdg_toplevel_set_title(window->xdg_toplevel, "Wayland EGL example");
	xdg_toplevel_add_listener(window->xdg_toplevel, &xdg_toplevel_listener,
							  NULL);

	// create native window
	window->region = wl_compositor_create_region(context->wl_compositor);

	wl_region_add(window->region, 0, 0, window->width, window->height);
	wl_surface_set_opaque_region(window->surface, window->region);

	wl_surface_set_buffer_scale(window->surface, window->content_scale);
	wl_surface_commit(window->surface);

	window->egl_window = wl_egl_window_create(
		window->surface, window->width * window->content_scale,
		window->height * window->content_scale);

	if (window->egl_window == EGL_NO_SURFACE) {
		LOG("No window !?\n");
		exit(1);
	} else {
		LOG("Window created !\n");
	}

	window->egl = create_egl_context(context->wl_display, window->egl_window);
	window->skia = skia_context_create_for_window(window);
	wl_display_dispatch_pending(context->wl_display);
	return window;
}