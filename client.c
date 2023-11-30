#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <wayland-client.h>

static const struct wl_output_listener output_listener = {
	.geometry = wl_output_handle_geometry;
	.mode = wl_output_handle_mode;
	.done = wl_output_handle_done;
	.scale = wl_output_handle_scale;
	.name = wl_output_handle_name;
	.description = wl_output_handle_description;
}

void registry_handle_global(void* data, struct wl_registry* registry, uint32_t name, const char* interface, uint32_t version) {
	printf("%u - %s\n", name, interface);

	if (strcmp(interface, wl_output_interface.name) == 0) {
		printf("Sending bind request on output!\n");
		struct wl_output* output = wl_registry_bind(registry, name, &wl_output_interface, version);
		wl_output_bind(output, )
	}
}

void registry_handle_global_remove(void* data, struct wl_registry* registry, uint32_t name) {
	// ignore
}

static const struct wl_registry_listener registry_listener = {
	.global = registry_handle_global,
	.global_remove = registry_handle_global_remove
};

int main() {
	// Connecting to display
	struct wl_display *display = wl_display_connect(NULL);
	if (!display) {
		fprintf(stderr, "ERROR: Failed to connect to Wayland display.\n");
		return 1;
	}
	printf("Connection established!\n");
	//

	struct wl_registry* registry = wl_display_get_registry(display);
	wl_registry_add_listener(registry, &registry_listener, NULL);
	wl_display_roundtrip(display); // Blocking until all current events are handled

	wl_display_disconnect(display);
	return 0;
}
