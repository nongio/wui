#ifndef _WUI_WL_H
#define _WUI_WL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wayland-client-core.h>
#include <wayland-client-protocol.h>
#include <wayland-client.h>
#include <xkbcommon/xkbcommon.h>

#include "xdg-shell-protocol.h"

#include "log.h"

struct pointer {
	struct wl_pointer *wlpointer;
	float x;
	float y;
	uint32_t last_click_button;
	uint32_t last_click_time;
	float last_click_x;
	float last_click_y;

	uint32_t button;
	//   NSTimeInterval	   last_timestamp;
	enum wl_pointer_button_state button_state;

	uint32_t axis_source;

	uint32_t serial;
	struct window *focus;
	struct window *captured;
};
struct cursor {
	struct wl_cursor *cursor;
	struct wl_surface *surface;
	struct wl_cursor_image *image;
	struct wl_buffer *buffer;
};

struct output {
	struct wayland_config *wlconfig;
	struct wl_output *output;
	uint32_t server_output_id;
	struct wl_list link;
	int alloc_x;
	int alloc_y;
	int width;
	int height;
	int transform;
	int scale;
	char *make;
	char *model;

	void *user_data;
};

struct window {
	struct wayland_config *wlconfig;
	int window_id;
	struct wl_list link;

	struct wl_surface *surface;
	struct xdg_surface *xdg_surface;
	struct xdg_toplevel *toplevel;
};
struct wayland_config {
	struct wl_display *wl_display;
	struct wl_registry *wl_registry;
	struct wl_compositor *wl_compositor;
	struct wl_seat *wl_seat;
	struct wl_keyboard *wl_keyboard;
	struct xdg_wm_base *xdg_wm_base;
	struct zwlr_layer_shell_v1 *layer_shell;

	int seat_version;

	struct wl_list output_list;
	int output_count;
	struct wl_list window_list;
	int window_count;

	// last event serial from pointer or keyboard
	uint32_t event_serial;

	// cursor
	struct wl_cursor_theme *cursor_theme;
	struct cursor *cursor;
	struct wl_surface *cursor_surface;

	// pointer
	struct pointer pointer;

	// keyboard
	struct xkb_context *xkb_context;

	struct {
		struct xkb_keymap *keymap;
		struct xkb_state *state;
		xkb_mod_mask_t control_mask;
		xkb_mod_mask_t alt_mask;
		xkb_mod_mask_t shift_mask;
	} xkb;

	int modifiers;
};

extern const struct wl_output_listener output_listener;
extern const struct wl_seat_listener seat_listener;
extern const struct wl_keyboard_listener keyboard_listener;
extern const struct wl_pointer_listener pointer_listener;

struct wayland_config *wui_wl_init();
struct wayland_config *wui_wl_context();

#endif