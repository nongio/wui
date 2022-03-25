#include "wl.h"

#define DOUBLECLICK_DELAY 300
#define DOUBLECLICK_MOVE_THREASHOLD 3

static void
pointer_handle_enter(void *data, struct wl_pointer *pointer, uint32_t serial,
					 struct wl_surface *surface, wl_fixed_t sx_w,
					 wl_fixed_t sy_w)
{
	struct wayland_config *wlconfig = (struct wayland_config *) data;

	struct window *window = (struct window *) wl_surface_get_user_data(surface);

	// if (window->ignoreMouse) {
	// 	return;
	// }

	wlconfig->pointer.focus = window;

	if (wlconfig->pointer.captured) {
		return;
	}

	// [(WaylandServer *) GSCurrentServer() initializeMouseIfRequired];

	LOG("[%d] pointer_handle_enter\n", window->window_id);

	float sx = wl_fixed_to_double(sx_w);
	float sy = wl_fixed_to_double(sy_w);

	if (window && wlconfig->pointer.serial) {
		
	}

	wlconfig->pointer.x = sx;
	wlconfig->pointer.y = sy;
	wlconfig->pointer.serial = serial;
	wlconfig->event_serial = serial;
}

static void
pointer_handle_leave(void *data, struct wl_pointer *pointer, uint32_t serial,
					 struct wl_surface *surface)
{
	struct wayland_config *wlconfig = (struct wayland_config *) data;
	struct window *window = (struct window *) wl_surface_get_user_data(surface);

	LOG("[%d] pointer_handle_leave\n", window->window_id);

	// if (window->ignoreMouse) {
	// 	return;
	// }

	if (wlconfig->pointer.focus == NULL) {
		return;
	}

	// [(WaylandServer *) GSCurrentServer() initializeMouseIfRequired];

	if (wlconfig->pointer.focus->window_id == window->window_id &&
		wlconfig->pointer.serial) {
		if (wlconfig->pointer.captured == NULL) {
			window = wlconfig->pointer.focus;
			
		}
		wlconfig->pointer.focus = NULL;
		wlconfig->pointer.serial = serial;
		wlconfig->event_serial = serial;
	}
}

// triggered when the cursor is over a surface
static void
pointer_handle_motion(void *data, struct wl_pointer *pointer, uint32_t time,
					  wl_fixed_t sx_w, wl_fixed_t sy_w)
{
    struct wayland_config *wlconfig = (struct wayland_config *) data;
	struct window *focused_window = wlconfig->pointer.focus;

	if (wlconfig->pointer.captured) {
		focused_window = wlconfig->pointer.captured;
	}
	if (focused_window == NULL) {// || focused_window->ignoreMouse) {
		return;
	}
	float sx = wl_fixed_to_double(sx_w);
	float sy = wl_fixed_to_double(sy_w);

	// wlconfig->pointer.last_timestamp = (NSTimeInterval) time / 1000.0;

	// [(WaylandServer *) GSCurrentServer() initializeMouseIfRequired];

	if (focused_window && wlconfig->pointer.serial) {
		
        

		unsigned int eventFlags;

		float deltaX = sx - wlconfig->pointer.x;
		float deltaY = sy - wlconfig->pointer.y;

		if (wlconfig->pointer.button_state == WL_POINTER_BUTTON_STATE_PRESSED) {

			// switch (wlconfig->pointer.button) {
			// case BTN_LEFT:
			// 	eventType = NSLeftMouseDragged;
			// 	break;
			// case BTN_RIGHT:
			// 	eventType = NSRightMouseDragged;
			// 	break;
			// case BTN_MIDDLE:
			// 	eventType = NSOtherMouseDragged;
			// 	break;
			// }
		}
	}

	wlconfig->pointer.x = sx;
	wlconfig->pointer.y = sy;
}

static void
pointer_handle_button(void *data, struct wl_pointer *pointer, uint32_t serial,
					  uint32_t time, uint32_t button, uint32_t state_w)
{
    struct wayland_config *wlconfig = (struct wayland_config *) data;
	
    
	unsigned int eventFlags;
	float deltaX = 0.0;
	float deltaY = 0.0;
	int clickCount = 1;
	int tick;
	int buttonNumber;
	enum wl_pointer_button_state state = (enum wl_pointer_button_state)state_w;

	struct window *window = wlconfig->pointer.focus;

	if (wlconfig->pointer.captured) {
		window = wlconfig->pointer.captured;
	}

	if (window == NULL) { // || window->ignoreMouse) {
		return;
	}
	// [(WaylandServer *) GSCurrentServer() initializeMouseIfRequired];

	eventFlags = wlconfig->modifiers;

	if (state == WL_POINTER_BUTTON_STATE_PRESSED) {
		wlconfig->pointer.button = button;
        LOG("[%d] WL_POINTER_BUTTON_STATE_PRESSED\n", window->window_id);
		if (window->toplevel) {

			// if point is in the title bar
//					xdg_toplevel_move(window->toplevel, wlconfig->seat, serial);
//					window->moving = YES;
//					return;
            // if point is in resize area

//					uint32_t edges = XDG_TOPLEVEL_RESIZE_EDGE_BOTTOM_RIGHT;
//						edges = XDG_TOPLEVEL_RESIZE_EDGE_BOTTOM_LEFT;
//						edges = XDG_TOPLEVEL_RESIZE_EDGE_BOTTOM_RIGHT;
//						edges = XDG_TOPLEVEL_RESIZE_EDGE_BOTTOM;

					// xdg_toplevel_resize(window->toplevel, wlconfig->seat,
					// 					serial, edges);
					// window->resizing = YES;
					// return;
		} // endif window->toplevel

// double click calculation 
		if (button == wlconfig->pointer.last_click_button &&
			time - wlconfig->pointer.last_click_time < DOUBLECLICK_DELAY &&
			fabsf(wlconfig->pointer.x - wlconfig->pointer.last_click_x) <
				DOUBLECLICK_MOVE_THREASHOLD &&
			fabsf(wlconfig->pointer.y - wlconfig->pointer.last_click_y) <
				DOUBLECLICK_MOVE_THREASHOLD) {
			wlconfig->pointer.last_click_time = 0;
			clickCount++;
		} else {
			wlconfig->pointer.last_click_button = button;
			wlconfig->pointer.last_click_time = time;
			wlconfig->pointer.last_click_x = wlconfig->pointer.x;
			wlconfig->pointer.last_click_y = wlconfig->pointer.y;
		}
		wlconfig->pointer.serial = serial;
		// switch (button) {
		// case BTN_LEFT:
		// 	eventType = NSLeftMouseDown;
		// 	break;
		// case BTN_RIGHT:
		// 	eventType = NSRightMouseDown;
		// 	break;
		// case BTN_MIDDLE:
		// 	eventType = NSOtherMouseDown;
		// 	break;
			// TODO: handle BTN_SIDE, BTN_EXTRA, BTN_FORWARD, BTN_BACK and other
			// constants in libinput.
			// We may just want to send NSOtherMouseDown and populate
			// buttonNumber with the libinput constant?
		// }
	} else if (state == WL_POINTER_BUTTON_STATE_RELEASED) {
		wlconfig->pointer.serial = 0;
		wlconfig->pointer.button = 0;
        LOG("[%d] WL_POINTER_BUTTON_STATE_RELEASED\n", window->window_id);
		// if (window->moving) {
		// 	window->moving = false;
		// 	return;
		// }
		// if (window->resizing) {
		// 	window->resizing = false;
		// 	return;
		// }
		// switch (button) {
		// case BTN_LEFT:
		// 	eventType = NSLeftMouseUp;
		// 	break;
		// case BTN_RIGHT:
		// 	eventType = NSRightMouseUp;
		// 	break;
		// case BTN_MIDDLE:
		// 	eventType = NSOtherMouseUp;
		// 	break;
		// }
	} else {
		return;
	}


	tick = serial;

	buttonNumber = button;

	

	// store button state for mouse move handlers
	wlconfig->pointer.button_state = state;
	// wlconfig->pointer.last_timestamp = timestamp;
	wlconfig->pointer.serial = serial;
	wlconfig->event_serial = serial;
}

// Discrete step information for scroll and other axes.
static void
pointer_handle_frame(void *data, struct wl_pointer *pointer)
{
}

// Source information for scroll and other axes.
static void
pointer_handle_axis_source(void *data, struct wl_pointer *pointer,
						   uint32_t axis_source)
{
    struct wayland_config *wlconfig = (struct wayland_config *) data;
	wlconfig->pointer.axis_source = axis_source;
}

// Stop notification for scroll and other axes.
static void
pointer_handle_axis_stop(void *data, struct wl_pointer *pointer, uint32_t time,
						 uint32_t axis)
{
}

// Discrete step information for scroll and other axes.
static void
pointer_handle_axis_discrete(void *data, struct wl_pointer *pointer,
							 uint32_t axis, int discrete)
{
}

// Scroll and other axis notifications.
static void
pointer_handle_axis(void *data, struct wl_pointer *pointer, uint32_t time,
					uint32_t axis, wl_fixed_t value)
{
	struct wayland_config *wlconfig = (struct wayland_config *) data;
	
	unsigned int eventFlags;
	float deltaX = 0.0;
	float deltaY = 0.0;
	int clickCount = 1;
	int buttonNumber;

	struct window *window = wlconfig->pointer.focus;
	// if (window->ignoreMouse) {
	// 	return;
	// }

	// [(WaylandServer *) GSCurrentServer() initializeMouseIfRequired];

	eventFlags = wlconfig->modifiers;

	if (wlconfig->pointer.axis_source != WL_POINTER_AXIS_SOURCE_WHEEL) {
		// axis_source == WL POINTER AXIS SOURCE FINGER
		// axis_source == WL POINTER AXIS SOURCE CONTINUOUS
		//  XXX the scroll is from trackpad we should calculate
		//  the momentumPhase
		// NSDebugLog(@"touch scroll");
	}

	// float mouse_scroll_multiplier = wlconfig->mouse_scroll_multiplier;
	/* For smooth-scroll events, we're not doing any cross-event or delta
	   calculations, as is done in button event handling. */
	// switch (axis) {
	// case WL_POINTER_AXIS_VERTICAL_SCROLL:
	// 	// deltaY = wl_fixed_to_double(value) * wlconfig->mouse_scroll_multiplier;
	// case WL_POINTER_AXIS_HORIZONTAL_SCROLL:
	// 	// deltaX = wl_fixed_to_double(value) * wlconfig->mouse_scroll_multiplier;
	// }

	
}

// the Seat category uses this listener
const struct wl_pointer_listener pointer_listener = {
	.enter = pointer_handle_enter,
    .leave = pointer_handle_leave,
    .motion = pointer_handle_motion,		 
    .button = pointer_handle_button,
    .axis = pointer_handle_axis,
    .frame = pointer_handle_frame,
	.axis_source = pointer_handle_axis_source,
    .axis_stop = pointer_handle_axis_stop,
	.axis_discrete = pointer_handle_axis_discrete
};

void cursor_hide()
{
	// to hide the cursor we set a NULL surface
	// wl_pointer_set_cursor(wlconfig->pointer.wlpointer, wlconfig->pointer.serial,
	// 					  NULL, 0, 0);
}

void cursor_show()
{
	// restore  the previous surface
	// wl_pointer_set_cursor(wlconfig->pointer.wlpointer, wlconfig->pointer.serial,
	// 					  wlconfig->cursor->surface,
	// 					  wlconfig->cursor->image->hotspot_x,
	// 					  wlconfig->cursor->image->hotspot_y);
}

struct cursor *standard_cursor_by_name(const char *cursor_name)
{
	// struct cursor *wayland_cursor = NULL;
	// if (strlen(cursor_name) != 0) {
	// 	LOG("load cursor from theme: %s\n", cursor_name);
	// 	wayland_cursor = (struct cursor *)malloc(sizeof(struct cursor));

	// 	wayland_cursor->cursor =
	// 		wl_cursor_theme_get_cursor(wlconfig->cursor_theme, cursor_name);

	// 	wayland_cursor->image = wayland_cursor->cursor->images[0];
	// 	wayland_cursor->buffer =
	// 		wl_cursor_image_get_buffer(wayland_cursor->image);

	// } 
    // return wayland_cursor;
}

void set_cursor(struct cursor *wayland_cursor)
{
	if (wayland_cursor == NULL) {
		return;
	}
	if (wayland_cursor->cursor == NULL) {
		return;
	}
	if (wayland_cursor->image == NULL) {
		return;
	}
	if (wayland_cursor->buffer == NULL) {
		return;
	}

	if (wayland_cursor->surface) {
		wl_surface_destroy(wayland_cursor->surface);
	}
	// wl_pointer_set_cursor(wlconfig->pointer.wlpointer, wlconfig->event_serial,
	// 					  wlconfig->cursor_surface,
	// 					  wayland_cursor->image->hotspot_x,
	// 					  wayland_cursor->image->hotspot_y);

	// wl_surface_attach(wlconfig->cursor_surface, wayland_cursor->buffer, 0, 0);
	// wl_surface_damage(wlconfig->cursor_surface, 0, 0,
	// 				  wayland_cursor->image->width,
	// 				  wayland_cursor->image->height);
	// wl_surface_commit(wlconfig->cursor_surface);

	// wlconfig->cursor = wayland_cursor;
}

void cursor_free(struct cursor *c)
{
	// the cursor should be deallocated
	// wl_cursor_destroy(c->cursor);
	wl_buffer_destroy(c->buffer);
	// free(cid);
}

void pointer_init()
{

	// wlconfig->cursor_theme = wl_cursor_theme_load(NULL, 24, wlconfig->shm);

	// wlconfig->cursor_surface =
	// 	wl_compositor_create_surface(wlconfig->compositor);

}

void pointer_options_changed()
{
	// wlconfig->mouse_scroll_multiplier = 1.0;
	// if (wlconfig->mouse_scroll_multiplier < 0.0001f) {
	// 	wlconfig->mouse_scroll_multiplier = 1.0f;
	// }
}