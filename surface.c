#include <stdio.h>
#include <stdlib.h>
#include <wayland-server.h>

#include "surface.h"

static struct wl_signal surface_commit;

void surface_commit_initialize() {
	wl_signal_init(&surface_commit);
}

void surface_commit_add_listener(struct wl_listener* listener) {
	wl_signal_add(&surface_commit, listener);
}

// I hate the fact i made 2 different functions for this, but im lazy lol
// Also, this will break if the client attaches a buffer, attaches another buffer, and then destroys the first one
static void on_pending_buffer_destroy(struct wl_listener* listener, void* data) {
	printf("Destroying pending buffer...\n");
	struct surface* surface;
	surface = wl_container_of(listener, surface, pending_buffer_destroy_listener);
	surface->pending_buffer = NULL;
}

static void on_buffer_destroy(struct wl_listener* listener, void* data) {
	printf("Destroying buffer...\n");
	struct surface* surface;
	surface = wl_container_of(listener, surface, buffer_destroy_listener);
	surface->buffer = NULL;
}
//

static void destroy_surface(struct wl_resource* resource) {
	struct surface* surface = wl_resource_get_user_data(resource);
	free(surface);
}

static void on_surface_destroy(struct wl_client* client, struct wl_resource* resource) {
	printf("Got request to destroy surface resource!\n");
	wl_resource_destroy(resource);
}

static void on_surface_attach(struct wl_client* client, struct wl_resource* resource, struct wl_resource* buffer, int32_t x, int32_t y) {
	printf("Got request to attach buffer to surface!\n");

	struct surface* surface = wl_resource_get_user_data(resource);
	surface->pending_buffer = buffer;

	wl_resource_add_destroy_listener(resource, &surface->pending_buffer_destroy_listener);
	surface->pending_buffer_destroy_listener.notify = on_pending_buffer_destroy;
}

static void on_surface_damage(struct wl_client* client, struct wl_resource* resource, int32_t x, int32_t y, int32_t width, int32_t height) {
	printf("Got damage request for surface!\n");
	//TODO
}

static void on_surface_frame(struct wl_client* client, struct wl_resource* resource, uint32_t callback) {
	printf("Got frame request for surface!\n");
	//TODO
}

static void on_surface_set_opaque_region(struct wl_client* client, struct wl_resource* resource, struct wl_resource* region) {
	printf("Got set_opaque_region request for surface!\n");
	//TODO
}

static void on_surface_set_input_region(struct wl_client* client, struct wl_resource* resource, struct wl_resource* region) {
	printf("Got set_input_region request for surface!\n");
	//TODO
}

static void on_surface_commit(struct wl_client* client, struct wl_resource* resource) {
	printf("Got commit request for surface!\n");

	struct surface* surface = wl_resource_get_user_data(resource);
	surface->buffer = surface->pending_buffer;
	
	wl_resource_add_destroy_listener(resource, &surface->buffer_destroy_listener);
	surface->buffer_destroy_listener.notify = on_buffer_destroy;

	wl_signal_emit(&surface_commit, (void*)surface);
}

static void on_surface_set_buffer_transform(struct wl_client* client, struct wl_resource* resource, int32_t transform) {
	printf("Got set_buffer_transform request for surface!\n");
	//TODO
}

static void on_surface_set_buffer_scale(struct wl_client* client, struct wl_resource* resource, int32_t scale) {
	printf("Got set_buffer_scale request for surface!\n");
	//TODO
}

static void on_surface_damage_buffer(struct wl_client* client, struct wl_resource* resource, int32_t x, int32_t y, int32_t width, int32_t height) {
	printf("Got damage_buffer request for surface!\n");
	//TODO
}

static void on_surface_offset(struct wl_client* client, struct wl_resource* resource, int32_t x, int32_t y) {
	printf("Got offset request for surface!\n");
	//TODO
}


static const struct wl_surface_interface surface_implementation = {
	.destroy = on_surface_destroy,
	.attach = on_surface_attach,
	.damage = on_surface_damage,
	.frame = on_surface_frame,
	.set_opaque_region = on_surface_set_opaque_region,
	.set_input_region = on_surface_set_input_region,
	.commit = on_surface_commit,
	.set_buffer_transform = on_surface_set_buffer_transform,
	.set_buffer_scale = on_surface_set_buffer_scale,
	.damage_buffer = on_surface_damage_buffer,
	.offset = on_surface_offset,
};

struct surface* surface_new(struct wl_client* client, uint32_t version, uint32_t id) {
	struct surface* surface = malloc(sizeof(*surface));
	if (!surface) {
		fprintf(stderr, "ERROR: Failed to allocate memory for new surface.\n");
		return NULL;
	}

	surface->buffer = surface->pending_buffer = NULL;

	surface->resource = wl_resource_create(client, &wl_surface_interface, version, id);
	if (!surface->resource) {
		fprintf(stderr, "ERROR: Failed to create resource for new surface.\n");
		goto error;
	}

	wl_resource_set_implementation(surface->resource, &surface_implementation, surface, destroy_surface);
	return surface;

error:
	free(surface);
	return NULL;
}
