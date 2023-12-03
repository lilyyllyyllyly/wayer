#include <wayland-server.h>

#ifndef SURFACE_H
#define SURFACE_H

void surface_commit_initialize();
void surface_commit_add_listener(struct wl_listener* listener);

void buffer_destroy_initialize();
void buffer_destroy_add_listener(struct wl_listener* listener);

struct surface {
	struct wl_resource* resource;

	struct wl_resource* buffer;
	struct wl_resource* pending_buffer;

	struct wl_listener buffer_destroy_listener;
	struct wl_listener pending_buffer_destroy_listener;
};

struct surface* surface_new(struct wl_client* client, uint32_t version, uint32_t id);

#endif

