#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <wayland-server.h>

#include "shm.h"

#define FORMATS_SIZE (sizeof(formats)/sizeof(formats[0]))
static const int32_t formats[] = {
	WL_SHM_FORMAT_ARGB8888,
	WL_SHM_FORMAT_XRGB8888
};

const int32_t* shm_get_formats() {
	return formats;
}

size_t shm_get_formats_size() {
	return sizeof(formats)/sizeof(formats[0]);
}

static void on_shm_create_pool(struct wl_client* client, struct wl_resource* resource, uint32_t id, int32_t fd, int32_t size) {
	printf("Got request to create pool!\n");
	// TODO
}

static const struct wl_shm_interface shm_implementation = {
	.create_pool = on_shm_create_pool,
};

static void on_shm_bind(struct wl_client* client, void* data, uint32_t version, uint32_t id) {
	struct shm* shm = data;
	struct wl_resource* resource = wl_resource_create(client, &wl_shm_interface, version, id);
	if (!resource) {
		fprintf(stderr, "ERROR: Failed to create resource while binding shm.\n");
		return;
	}
	
	wl_resource_set_implementation(resource, &shm_implementation, shm, NULL);

	// Send format events
	for (int i = 0; i < (int)FORMATS_SIZE; ++i) {
		wl_shm_send_format(resource, formats[i]);
	}
}

struct shm* shm_new(struct wl_display* display) {
	struct shm* shm = malloc(sizeof(*shm));
	if (!shm) {
		fprintf(stderr, "ERROR: Failed to allocate memory for new shm.\n");
		return NULL;
	}

	shm->global = wl_global_create(display, &wl_shm_interface, wl_shm_interface.version, shm, on_shm_bind);
	if (!shm->global) {
		fprintf(stderr, "ERROR: Failed to create global for new shm.\n");
		goto error;
	}

	return shm;

error:
	free(shm);
	return NULL;
}

