#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <signal.h>
#include <wayland-server.h>

#include "compositor.h"
#include "shm.h"

struct wl_display* display;
struct shm* shm;

void on_interrupt() {
	if (!display) return;

	printf("\nSIGINT caught, terminating display...\n");
	wl_display_terminate(display);
}

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
	for (int i = 0; i < shm_get_formats_size(); ++i) {
		wl_display_add_shm_format(display, shm_get_formats()[i]);
	}

	// Creating globals
	if (compositor_new(display)) {
		goto error;
	}
//	if (!(shm = shm_new(display))) {
//		goto error;
//	}
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
