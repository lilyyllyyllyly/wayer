#include <stdint.h>
#include <wayland-server.h>

#ifndef SHM_H
#define SHM_H

const int32_t* shm_get_formats();
size_t shm_get_formats_size();

struct shm {
	struct wl_global* global;
};

struct shm* shm_new(struct wl_display* display);

#endif

