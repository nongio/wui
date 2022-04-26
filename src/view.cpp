#include "view.h"

struct wui_view *
view_create(int width, int height)
{

	struct wayland_config *wlconfig = wui_wl_context();

	struct wui_view *view =
		(struct wui_view *) calloc(1, sizeof(struct wui_view));
	
	view_init(view, width, height);
	return view;
}

struct wui_subview *subview_create(struct wl_surface *parent, int x, int y, int width, int height)
{
	struct wayland_config *wlconfig = wui_wl_context();

	struct wui_subview *subview =
		(struct wui_subview *) calloc(1, sizeof(struct wui_subview));
	
	view_init(&subview->base, width, height);


	subview->subsurface = wl_subcompositor_get_subsurface (
        wlconfig->wl_subcompositor,
        subview->base.surface,
        parent);

    wl_subsurface_set_desync(subview->subsurface);

	wl_subsurface_set_position(subview->subsurface, x, y);
	wl_subsurface_place_above(subview->subsurface, parent);

	view_init_buffer(&subview->base);

    wl_surface_commit (subview->base.surface);
    wl_surface_commit(parent);
    wl_display_roundtrip(wlconfig->wl_display);


	return subview;
}

void view_init(struct wui_view *view, int w, int h) {
	struct wayland_config *wlconfig = wui_wl_context();

	view->wl = wlconfig;
	view->width = w;
	view->height = h;
	view->content_scale = 2;
	view->surface = wl_compositor_create_surface(wlconfig->wl_compositor);
    wl_surface_set_buffer_scale(view->surface, 2);
	if (view->surface == NULL) {
		LOG("No Compositor surface ! Yay....\n");
		exit(1);
	} else {
		LOG("Got a compositor surface !\n");
	}
	
}

void view_init_buffer(struct wui_view *view) {
	// 
	wl_surface_set_user_data(view->surface, view);

	// graphic context
	view->egl_window = wl_egl_window_create(
		view->surface, view->width * view->content_scale,
		view->height * view->content_scale);

	if (view->egl_window == EGL_NO_SURFACE) {
		LOG("No window !?\n");
		exit(1);
	} else {
		LOG("egl window created !\n");
	}

	view->egl = create_egl_context(view->wl->wl_display, view->egl_window);
	view->skia = skia_context_create_for_view(view);


	wl_display_dispatch_pending(view->wl->wl_display);
}