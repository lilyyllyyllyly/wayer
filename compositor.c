#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <wayland-server.h>

#include "surface.h"

static struct wl_global* global;

static void on_compositor_create_surface(struct wl_client* client, struct wl_resource* resource, uint32_t id) {
	printf("Got request to create surface!\n");

	struct surface* surface = surface_new(client, wl_resource_get_version(resource), id);
	if (!surface->resource) {
		wl_resource_post_no_memory(resource);
		return;
	}
}

static void on_compositor_create_region(struct wl_client* client, struct wl_resource* resource, uint32_t id) {
	printf("Got request to create region!\n");
	// TODO
}

static const struct wl_compositor_interface compositor_implementation = {
	.create_surface = on_compositor_create_surface,
	.create_region = on_compositor_create_region,
};

static void on_compositor_bind(struct wl_client* client, void* data, uint32_t version, uint32_t id) {
	printf("Bind request for compositor received!\n");

	struct wl_resource* resource = wl_resource_create(client, &wl_compositor_interface, version, id);
	if (!resource) {
		wl_client_post_no_memory(client);
		fprintf(stderr, "ERROR: Failed to create resource while binding compositor.\n");
		return;
	}

	wl_resource_set_implementation(resource, &compositor_implementation, NULL, NULL);
}

int compositor_new(struct wl_display* display) {
	global = wl_global_create(display, &wl_compositor_interface, wl_compositor_interface.version, NULL, on_compositor_bind);
	if (!global) {
		fprintf(stderr, "ERROR: Failed to create global for new compositor.\n");
		return 1;
	}

	return 0;
}
