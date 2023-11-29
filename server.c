#include <stdio.h>
#include <stdint.h>
#include <signal.h>

#ifndef WAYLAND_SERVER_H_INCLUDED
#define WAYLAND_SERVER_H_INCLUDED
#include <wayland-server.h>
#endif

#include "output.h"

struct wl_display* display;

struct state {
	char* test;
};

void handle_interrupt() {
	if (!display) return;

	printf("\nSIGINT caught, terminating display...\n");
	wl_display_terminate(display);
}

int main() {
	signal(SIGINT, handle_interrupt);

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
	struct state output_state = { "hi" };
	wl_global_create(display, &wl_output_interface, wl_output_interface.version, (void*)&output_state, wl_output_handle_bind);
	//

	printf("Running Wayland display on socket %s...\n", socket);
	wl_display_run(display); // Blocking until wl_display_terminate

	wl_display_destroy(display);
	printf("Display destroyed. Done :3\n");

	return 0;
}
