#include "log.h"
#include "wl.h"

static void
handle_geometry(void *data, struct wl_output *wl_output, int x, int y,
				int physical_width, int physical_height, int subpixel,
				const char *make, const char *model, int transform)
{
	LOG("output: handle_geometry\n");
	struct output *output = (struct output *) data;

	output->alloc_x = x;
	output->alloc_y = y;
	output->transform = transform;

	if (output->make)
		free(output->make);
	output->make = strdup(make);

	if (output->model)
		free(output->model);
	output->model = strdup(model);
}

static void
handle_done(void *data, struct wl_output *wl_output)
{
	LOG("output: handle_done\n");
}

static void
handle_scale(void *data, struct wl_output *wl_output, int32_t scale)
{
	LOG("output: handle_scale %d\n", scale);
	struct output *output = (struct output *) data;

	output->scale = scale;
}

static void
handle_mode(void *data, struct wl_output *wl_output, uint32_t flags, int width,
			int height, int refresh)
{
	LOG("output: handle_mode\n");
	struct output *output = (struct output *) data;

	if (flags & WL_OUTPUT_MODE_CURRENT) {
		output->width = width;
		output->height = height;
		LOG("output: handle_mode output=%dx%d\n", width, height);
	}
}

const struct wl_output_listener output_listener = {.geometry = handle_geometry,
												   .mode = handle_mode,
												   .done = handle_done,
												   .scale = handle_scale,};