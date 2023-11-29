#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <wayland-client.h>

void registry_handle_global(void* data, struct wl_registry* registry, uint32_t name, char* interface, uint32_t version) {
	printf("%u - %s\n", name, interface);
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
