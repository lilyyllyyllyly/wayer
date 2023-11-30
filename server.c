#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <signal.h>
#include <wayland-server.h>

#include "output.h"

struct wl_display* display;

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

	// Creating a global
	struct output* output = output_new(display);
	//

	printf("Running Wayland display on socket %s...\n", socket);
	wl_display_run(display); // Blocking until wl_display_terminate

	wl_display_destroy(display);
	printf("Display destroyed. Done :3\n");
	if (output) {
		free(output);
	}

	return 0;
}
