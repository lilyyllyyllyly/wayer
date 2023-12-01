#include <wayland-server.h>

#ifndef COMPOSITOR_H
#define COMPOSITOR_H

/* creates a new compositor global, returns 1 in case of error */
int compositor_new(struct wl_display* display);

#endif

