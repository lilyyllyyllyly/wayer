#ifndef WAYLAND_SERVER_H_INCLUDED
#define WAYLAND_SERVER_H_INCLUDED
#include <wayland-server.h>
#endif

#ifndef OUTPUT_H_INCLUDED
#define OUTPUT_H_INCLUDED

struct output {
	struct wl_resource* resource;
	void* state;
};

void wl_output_handle_resource_destroy(struct wl_resource* resource);

void wl_output_handle_release(struct wl_client* client, struct wl_resource* resource);

const struct wl_output_interface wl_output_implementation = {
	.release = wl_output_handle_release
};

void wl_output_handle_bind(struct wl_client* client, void* data, uint32_t version, uint32_t id);

#endif
