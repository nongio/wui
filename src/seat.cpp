#include "wl.h"

static void
seat_handle_capabilities(void *data, struct wl_seat *wl_seat,
						 uint32_t caps)
{
	struct wayland_config *wlconfig = (wayland_config *) data;

	if ((caps & WL_SEAT_CAPABILITY_POINTER) && !wlconfig->pointer.wlpointer) {
		wlconfig->pointer.wlpointer = wl_seat_get_pointer(wl_seat);
		wl_pointer_set_user_data(wlconfig->pointer.wlpointer, wlconfig);
		wl_pointer_add_listener(wlconfig->pointer.wlpointer, &pointer_listener,
								wlconfig);
	} else if (!(caps & WL_SEAT_CAPABILITY_POINTER) &&
			   wlconfig->pointer.wlpointer) {
		if (wlconfig->seat_version >= WL_POINTER_RELEASE_SINCE_VERSION)
			wl_pointer_release(wlconfig->pointer.wlpointer);
		else
			wl_pointer_destroy(wlconfig->pointer.wlpointer);
		wlconfig->pointer.wlpointer = NULL;
	}

	wl_display_dispatch_pending(wlconfig->wl_display);
	wl_display_flush(wlconfig->wl_display);

	if ((caps & WL_SEAT_CAPABILITY_KEYBOARD) && !wlconfig->wl_keyboard) {
		wlconfig->wl_keyboard = wl_seat_get_keyboard(wl_seat);
		wl_keyboard_set_user_data(wlconfig->wl_keyboard, wlconfig);
		wl_keyboard_add_listener(wlconfig->wl_keyboard, &keyboard_listener,
								 wlconfig);
	} else if (!(caps & WL_SEAT_CAPABILITY_KEYBOARD) && wlconfig->wl_keyboard) {
		if (wlconfig->seat_version >= WL_KEYBOARD_RELEASE_SINCE_VERSION)
			wl_keyboard_release(wlconfig->wl_keyboard);
		else
			wl_keyboard_destroy(wlconfig->wl_keyboard);
		wlconfig->wl_keyboard = NULL;
	}

#if 0
    if ((caps & WL_SEAT_CAPABILITY_TOUCH) && !input->touch) {
	input->touch = wl_seat_get_touch(seat);
	wl_touch_set_user_data(input->touch, input);
	wl_touch_add_listener(input->touch, &touch_listener, input);
    } else if (!(caps & WL_SEAT_CAPABILITY_TOUCH) && input->touch) {
	if (input->seat_version >= WL_TOUCH_RELEASE_SINCE_VERSION)
	    wl_touch_release(input->touch);
	else
	    wl_touch_destroy(input->touch);
	input->touch = NULL;
    }
#endif
}

const struct wl_seat_listener seat_listener = {
	.capabilities = seat_handle_capabilities,
};