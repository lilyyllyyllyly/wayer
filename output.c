#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include "output.h"

void wl_output_handle_resource_destroy(struct wl_resource* resource) {
	// TODO: clean up resource (??)
	printf("uhhh client destroyed a resource wtf do i do\n");
}

void wl_output_handle_release(struct wl_client* client, struct wl_resource* resource) {
	wl_resource_destroy(resource);
}

void wl_output_handle_bind(struct wl_client* client, void* data, uint32_t version, uint32_t id) {
	struct output* client_output = calloc(1, sizeof(client_output));
	struct wl_resource* resource;

	resource = wl_resource_create(client, &wl_output_interface, wl_output_interface.version, id);
	if (!resource) {
		wl_client_post_no_memory(client);
		fprintf(stderr, "ERROR: Failed to create resource while binding output.\n");
		return;
	}

	wl_resource_set_implementation(resource, &wl_output_implementation, data, wl_output_handle_resource_destroy);

	client_output->resource = resource;
	client_output->state = data;
}

