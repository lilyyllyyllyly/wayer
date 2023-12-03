#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <signal.h>
#include <wayland-server.h>

#include "compositor.h"
#include "surface.h"

struct wl_display* display;
struct wl_global* wm_base_global;

#define SHM_FORMATS_SIZE (sizeof(shm_formats)/sizeof(shm_formats[0]))
static const int32_t shm_formats[] = {
	WL_SHM_FORMAT_ARGB8888,
	WL_SHM_FORMAT_XRGB8888
};

void on_interrupt() {
	if (!display) return;

	printf("\nSIGINT caught, terminating display...\n");
	wl_display_terminate(display);
}

void on_surface_commit(struct wl_listener* listener, void* data) {
	printf("server: listened to surface commit\n");
	struct surface* surface = data;
	struct wl_shm_buffer* buffer = wl_shm_buffer_get(surface->buffer);
	printf("width: %d\n", wl_shm_buffer_get_width(buffer));
	printf("height: %d\n", wl_shm_buffer_get_height(buffer));
}

struct wl_listener surface_commit_listener = {
	.notify = on_surface_commit,
};

int main() {
	signal(SIGINT, on_interrupt);

	// Creating display
	display = wl_display_create();
	if (!display) {
		fprintf(stderr, "ERROR: couldn't create Wayland display.\n");
		return 1;
	}

	// Adding socket to display
	const char* socket = wl_display_add_socket_auto(display);
	if (!socket) {
		fprintf(stderr, "ERROR: couldn't add socket to Wayland display.\n");
		return 1;
	}

	// Shared Memory
	wl_display_init_shm(display);
	for (int i = 0; i < (int)SHM_FORMATS_SIZE; ++i) {
		wl_display_add_shm_format(display, shm_formats[i]);
	}

	surface_commit_initialize();
	surface_commit_add_listener(&surface_commit_listener); // Listen to commit from surfaces
	
	// Creating Compositor
	if (compositor_new(display)) {
		goto error;
	}
	//

	printf("Running Wayland display on socket %s...\n", socket);
	wl_display_run(display); // Blocking until wl_display_terminate

	wl_display_destroy(display);
	printf("Display destroyed. Done :3\n");

	return 0;

error:
	wl_display_destroy(display);
	printf("An error occured. Display destroyed :(\n");
	return 1;
}
