#include "wl.h"

struct wayland_config *wlconfig;


// wayland wm base ping handler

static void
xdg_wm_base_ping(void *data, struct xdg_wm_base *xdg_wm_base, uint32_t serial)
{
	xdg_wm_base_pong(xdg_wm_base, serial);
}

const struct xdg_wm_base_listener xdg_wm_base_listener = {
	.ping = xdg_wm_base_ping,
};

// wayland registry handler

static void
global_registry_handler(void *data, struct wl_registry *registry, uint32_t id,
						const char *interface, uint32_t version)
{
	LOG("Got a registry event for %s id %d\n", interface, id);
	struct wayland_config *wlconfig = (struct wayland_config *)data;
	if (strcmp(interface, "wl_compositor") == 0) {
		LOG("got a compositor\n");
		wlconfig->wl_compositor = (struct wl_compositor *) wl_registry_bind(
			registry, id, &wl_compositor_interface, 3);

	} else if (strcmp(interface, xdg_wm_base_interface.name) == 0) {

		wlconfig->xdg_wm_base = (struct xdg_wm_base *) wl_registry_bind(
			registry, id, &xdg_wm_base_interface, 1);
		xdg_wm_base_add_listener(wlconfig->xdg_wm_base, &xdg_wm_base_listener,
								 wlconfig
								 );
	} else if (strcmp(interface, wl_output_interface.name) == 0) {
		struct output *output = (struct output *) malloc(sizeof(struct output));
		memset(output, 0, sizeof(struct output));
		output->wlconfig = wlconfig;
		output->scale = 1;
		output->output = (struct wl_output*) wl_registry_bind(registry, id, &wl_output_interface, 2);
		output->server_output_id = id;
		wl_list_insert(wlconfig->output_list.prev, &output->link);
		wlconfig->output_count++;
		wl_output_add_listener(output->output, &output_listener, output);
	} else if (strcmp(interface, wl_seat_interface.name) == 0) {
		wlconfig->pointer.wlpointer = NULL;
		wlconfig->seat_version = version;
		wlconfig->wl_seat = (struct wl_seat *) wl_registry_bind(wlconfig->wl_registry, id, &wl_seat_interface, 1);
		wl_seat_add_listener(wlconfig->wl_seat, &seat_listener, wlconfig);
	}
	//  else if(strcmp(interface, sc_shell_unstable_v1_interface.name) == 0) {
	// 	SCShell = wl_registry_bind(registry, id,
	// 			&sc_shell_unstable_v1_interface, 1);
	// }
}

// wayland registry handler on remove

static void
global_registry_remover(void *data, struct wl_registry *registry, uint32_t id)
{
	LOG("Got a registry loosing event for %d\n", id);
}

const struct wl_registry_listener wl_registry_listener = {
	global_registry_handler, global_registry_remover};

struct wayland_config *
wui_wl_init()
{

 	wlconfig = (struct wayland_config *) calloc(1, sizeof(struct wayland_config));
	
	wl_list_init(&wlconfig->output_list);
  	wl_list_init(&wlconfig->window_list);

	wlconfig->wl_display = wl_display_connect(NULL);

	if (wlconfig->wl_display == NULL) {
		LOG("Can't connect to wayland display\n");
		exit(1);
	}

	wlconfig->wl_registry = wl_display_get_registry(wlconfig->wl_display);
	wl_registry_add_listener(wlconfig->wl_registry, &wl_registry_listener,
							 wlconfig);

	// This call the attached listener global_registry_handler
	wl_display_dispatch(wlconfig->wl_display);
	wl_display_roundtrip(wlconfig->wl_display);

	// If at this point, global_registry_handler didn't set the
	// compositor, nor the shell, bailout !
	if (wlconfig->wl_compositor == NULL || wlconfig->xdg_wm_base == NULL) {
		LOG("No compositor !? No XDG !! There's NOTHING in here !\n");
		exit(1);
	} else {
		LOG("Okay, we got a compositor and a shell... That's something !\n");
	}

	return wlconfig;
}

struct wayland_config *
wui_wl_context()
{
	if (wlconfig == NULL) {
		wui_wl_init();
	}
	return wlconfig;
}