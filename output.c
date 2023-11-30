#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <wayland-server.h>

#include "output.h"

static void on_output_resource_destroy(struct wl_resource* resource) {
	// TODO: clean up resource (??)
	printf("uhhh client destroyed a resource wtf do i do\n");
}

static void on_output_release(struct wl_client* client, struct wl_resource* resource) {
	wl_resource_destroy(resource);
}

static const struct wl_output_interface output_implementation = {
	.release = on_output_release
};

static void on_output_bind(struct wl_client* client, void* data, uint32_t version, uint32_t id) {
	printf("Bind request for output received!\n");

	struct output* output = data;
	struct wl_resource* resource;

	resource = wl_resource_create(client, &wl_output_interface, wl_output_interface.version, id);
	if (!resource) {
		wl_client_post_no_memory(client);
		fprintf(stderr, "ERROR: Failed to create resource while binding output.\n");
		return;
	}

	wl_resource_set_implementation(resource, &output_implementation, data, wl_output_handle_resource_destroy);

	output->resource = resource;

	OUTPUT_SEND_GEOMETRY(output);
}

struct output* output_new(struct wl_display* display) {
	struct output* output = malloc(sizeof(*output));
	if (!output) {
		fprintf(stderr, "ERROR: Failed to allocate memory for new output.\n");
		goto error;
	}

	output->global = wl_global_create(display, &wl_output_interface, wl_output_interface.version, output, on_output_bind);

	if (!output->global) {
		fprintf(stderr, "ERROR: Failed to create global for new output.\n");
		goto error;
	}

	return output;

error:
	free(output);
	return NULL;
}

