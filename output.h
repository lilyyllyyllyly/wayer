#include <wayland-server.h>
#include <stdint.h>

#ifndef OUTPUT_H
#define OUTPUT_H

#define OUTPUT_SEND_GEOMETRY(OUT) wl_output_send_geometry(OUT->resource, OUT->x, OUT->y, OUT->physical_w, OUT->physical_h, OUT->subpixel, OUT->make, OUT->model, OUT->transform)


struct output {
	struct wl_resource* resource;
	struct wl_global* global;
	
	// geometry event
	int32_t x, y;
	int32_t physical_w, physical_h;
	int32_t subpixel, transform;
	const char* make;
	const char* model;
};

struct output* output_new(struct wl_display* display);

#endif
