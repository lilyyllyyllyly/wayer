#include <stdio.h>
#include <stdint.h>
#include <wayland-server.h>
#include "xdg-shell-server-protocol.h"

static void on_xdg_surface_destroy(struct wl_client* client, struct wl_resource* resource) {
	printf("Got destroy request for xdg_surface!\n");
	wl_resource_destroy(resource);
}

static void on_xdg_surface_get_toplevel(struct wl_client* client, struct wl_resource* resource, uint32_t id) {
	printf("Got get_toplevel request for xdg_surface!\n");
	//TODO
}

static void on_xdg_surface_get_popup(struct wl_client* client, struct wl_resource* resource, uint32_t id, struct wl_resource* parent, struct wl_resource* positioner) {
	printf("Got get_popup request for xdg_surface!\n");
	//TODO
}

static void on_xdg_surface_set_window_geometry(struct wl_client* client, struct wl_resource* resource, int32_t x, int32_t y, int32_t width, int32_t height) {
	printf("Got set_window_geometry request for xdg_surface!\n");
	//TODO
}

static void on_xdg_surface_ack_configure(struct wl_client* client, struct wl_resource* resource, uint32_t serial) {
	printf("Got ack_configure request for xdg_surface!\n");
	//TODO
}

static const struct xdg_surface_interface xdg_surface_implementation = {
	.destroy = on_xdg_surface_destroy,
	.get_toplevel = on_xdg_surface_get_toplevel,
	.get_popup = on_xdg_surface_get_popup,
	.set_window_geometry = on_xdg_surface_set_window_geometry,
	.ack_configure = on_xdg_surface_ack_configure,
};

static void on_wm_base_destroy(struct wl_client* client, struct wl_resource* resource) {
	wl_resource_destroy(resource);
}

static void on_wm_base_create_positioner(struct wl_client* client, struct wl_resource* resource, uint32_t id) {
	printf("Got wm_base create_positioner request!\n");
	//TODO
}

static void on_wm_base_get_xdg_surface(struct wl_client* client, struct wl_resource* resource, uint32_t id, struct wl_resource* surface_resource) {
	printf("Got wm_base get_xdg_surface request!\n");

//	struct wl_resource* xdg_surface_resource;
//	xdg_surface_resource = wl_resource_create(client, &xdg_surface_interface, wl_resource_get_version(resource), id);
//	if (!xdg_surface_resource) {
//		fprintf(stderr, "ERROR: Failed to create resource for new xdg_surface.\n");
//		return;
//	}
//
//	wl_resource_set_implementation(resource, &xdg_surface_implementation, NULL, NULL);
}

static void on_wm_base_pong(struct wl_client* client, struct wl_resource* resource, uint32_t serial) {
	printf("Got wm_base pong request!\n");
	//TODO
}

static const struct xdg_wm_base_interface wm_base_implementation = {
	.destroy = on_wm_base_destroy,
	.create_positioner = on_wm_base_create_positioner,
	.get_xdg_surface = on_wm_base_get_xdg_surface,
	.pong = on_wm_base_pong,
};

static void on_wm_base_bind(struct wl_client* client, void* data, uint32_t version, uint32_t id) {
	printf("Bind request for wm_base received!\n");
	
	struct wl_resource* resource = wl_resource_create(client, &xdg_wm_base_interface, version, id);
	if (!resource) {
		wl_client_post_no_memory(client);
		fprintf(stderr, "ERROR: Failed to create resource while binding xdg_wm_base.\n");
		return;
	}

	wl_resource_set_implementation(resource, &wm_base_implementation, NULL, NULL);
}

struct wl_global* wm_base_new(struct wl_display* display) {
	return wl_global_create(display, &xdg_wm_base_interface, xdg_wm_base_interface.version, NULL, &on_wm_base_bind);
}
