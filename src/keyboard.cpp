#include "wl.h"
#include <unistd.h>
#include <sys/mman.h>
#include <linux/input.h>
#include <xkbcommon/xkbcommon.h>

static void
keyboard_handle_keymap(void *data, struct wl_keyboard *keyboard,
					   uint32_t format, int fd, uint32_t size)
{
	LOG("keyboard: keyboard_handle_keymap\n");
	struct wayland_config *wlconfig = (struct wayland_config *)data;
	struct xkb_keymap *keymap;
	struct xkb_state *state;
	char *map_str;

	if (!data) {
		close(fd);
		return;
	}

	if (format != WL_KEYBOARD_KEYMAP_FORMAT_XKB_V1) {
		close(fd);
		return;
	}

	map_str = (char *) mmap(NULL, size, PROT_READ, MAP_SHARED, fd, 0);
	if (map_str == MAP_FAILED) {
		close(fd);
		return;
	}

	wlconfig->xkb_context = xkb_context_new(XKB_CONTEXT_NO_FLAGS);
	if (wlconfig->xkb_context == NULL) {
		fprintf(stderr, "Failed to create XKB context\n");
		return;
	}

	keymap = xkb_keymap_new_from_string(wlconfig->xkb_context, map_str,
										XKB_KEYMAP_FORMAT_TEXT_V1, XKB_KEYMAP_COMPILE_NO_FLAGS);
	munmap(map_str, size);
	close(fd);

	if (!keymap) {
		fprintf(stderr, "failed to compile keymap\n");
		return;
	}

	state = xkb_state_new(keymap);
	if (!state) {
		fprintf(stderr, "failed to create XKB state\n");
		xkb_keymap_unref(keymap);
		return;
	}

	xkb_keymap_unref(wlconfig->xkb.keymap);
	xkb_state_unref(wlconfig->xkb.state);
	wlconfig->xkb.keymap = keymap;
	wlconfig->xkb.state = state;

	wlconfig->xkb.control_mask =
		1 << xkb_keymap_mod_get_index(wlconfig->xkb.keymap, "Control");
	wlconfig->xkb.alt_mask =
		1 << xkb_keymap_mod_get_index(wlconfig->xkb.keymap, "Mod1");
	wlconfig->xkb.shift_mask =
		1 << xkb_keymap_mod_get_index(wlconfig->xkb.keymap, "Shift");
}

static void
keyboard_handle_enter(void *data, struct wl_keyboard *keyboard, uint32_t serial,
					  struct wl_surface *surface, struct wl_array *keys)
{
	struct wayland_config *wlconfig = (struct wayland_config *)data;
	wlconfig->event_serial = serial;
}

static void
keyboard_handle_leave(void *data, struct wl_keyboard *keyboard, uint32_t serial,
					  struct wl_surface *surface)
{
    struct wayland_config *wlconfig = (struct wayland_config *)data;
	wlconfig->event_serial = serial;
}

static void
keyboard_handle_modifiers(void *data, struct wl_keyboard *keyboard,
						  uint32_t serial, uint32_t mods_depressed,
						  uint32_t mods_latched, uint32_t mods_locked,
						  uint32_t group)
{
    struct wayland_config *wlconfig = (struct wayland_config *)data;
	wlconfig->event_serial = serial;
	xkb_mod_mask_t mask;

	/* If we're not using a keymap, then we don't handle PC-style modifiers */
	if (!wlconfig->xkb.keymap)
		return;

	xkb_state_update_mask(wlconfig->xkb.state, mods_depressed, mods_latched,
						  mods_locked, 0, 0, group);
}

static void
keyboard_handle_key(void *data, struct wl_keyboard *keyboard, uint32_t serial,
					uint32_t time, uint32_t key, uint32_t state_w)
{
    struct wayland_config *wlconfig = (struct wayland_config *)data;
	wlconfig->event_serial = serial;
	uint32_t code, num_syms;
	const xkb_keysym_t *syms;
	xkb_keysym_t sym;
	struct window *window = wlconfig->pointer.focus;

	if (!window)
		return;

	code = 0;
	// if (key == 28) {
	// 	sym = NSCarriageReturnCharacter;
	// } else if (key == 14) {
	// 	sym = NSDeleteCharacter;
	// } else {
	// 	code = key + 8;

	// 	num_syms = xkb_state_key_get_syms(wlconfig->xkb.state, code, &syms);

	// 	sym = XKB_KEY_NoSymbol;
	// 	if (num_syms == 1)
	// 		sym = syms[0];
	// }

	// NSString *s = [NSString stringWithUTF8String:&sym];
	// NSEventType eventType;

	// if (state == WL_KEYBOARD_KEY_STATE_PRESSED) {
	// 	eventType = NSKeyDown;
	// } else {
	// 	eventType = NSKeyUp;
	// }
}

static void
keyboard_handle_repeat_info(void *data, struct wl_keyboard *keyboard,
							int32_t rate, int32_t delay)
{
}

const struct wl_keyboard_listener keyboard_listener = {
	.keymap = keyboard_handle_keymap,
    .enter = keyboard_handle_enter,
	.leave = keyboard_handle_leave,	   
    .key = keyboard_handle_key,
	.modifiers = keyboard_handle_modifiers, 
    .repeat_info = keyboard_handle_repeat_info};