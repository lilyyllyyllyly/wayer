#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <signal.h>
#include <wayland-server.h>
#include <raylib.h>

#include "compositor.h"
#include "surface.h"

#define PPM_MAGIC "P3"
#define PPM_MAX_COLOR 255
#define PPM_FILETYPE ".ppm"

#define RL_DEFAULT_WIDTH  128
#define RL_DEFAULT_HEIGHT 128
#define RL_TITLE "wayer"

#define SHM_FORMATS_SIZE (sizeof(shm_formats)/sizeof(shm_formats[0]))
static const int32_t shm_formats[] = {
	WL_SHM_FORMAT_ARGB8888,
	WL_SHM_FORMAT_XRGB8888
};

int return_value = 0;
struct wl_display* display;

void on_interrupt() {
	if (!display) return;

	printf("\nSIGINT caught, terminating display...\n");
	wl_display_terminate(display);
}

void draw_buffer(int32_t* pixels, int32_t width, int32_t height, int32_t stride, int32_t format) {
	if (format != 0) {
		fprintf(stderr, "ERROR: Buffer format not implemented (only ARGB8888 is).\n");
		return;
	}

	// Scale window
	SetWindowSize(width, height);

	// Generate image from pixel data (in a very stupid way)
	Image img = GenImageColor(width, height, WHITE);
	for (int i = 0; i < (int)(stride*height/sizeof(int32_t)); ++i) {
		Color color = { (pixels[i] >> (8*2)) & 0xFF,
				(pixels[i] >> (8*1)) & 0xFF,
				(pixels[i] >> (8*0)) & 0xFF, 0xFF };
		ImageDrawPixel(&img, i%width, i/width, color);
	}

	// Generate and draw texture
	Texture tex = LoadTextureFromImage(img);
	UnloadImage(img);

	BeginDrawing();
	DrawTexture(tex, 0, 0, WHITE);
	EndDrawing();
}

void on_surface_commit(struct wl_listener* listener, void* data) {
	printf("server: listened to surface commit\n");
	struct surface* surface = data;

	// Getting buffer image data and drawing
	struct wl_shm_buffer* buffer = wl_shm_buffer_get(surface->buffer);
	wl_shm_buffer_begin_access(buffer);

	int32_t width   = wl_shm_buffer_get_width(buffer);
	int32_t height  = wl_shm_buffer_get_height(buffer);
	int32_t stride  = wl_shm_buffer_get_stride(buffer);
	uint32_t format = wl_shm_buffer_get_format(buffer);
	int32_t* buff_data = wl_shm_buffer_get_data(buffer);
	draw_buffer(buff_data, width, height, stride, format);

	wl_shm_buffer_end_access(buffer);
}

void on_buffer_destroy(struct wl_listener* listener, void* data) {
	// Clear window (this whole signal makes no sense i know)
	BeginDrawing();
	ClearBackground(BLACK);
	EndDrawing();
}

struct wl_listener surface_commit_listener = {
	.notify = on_surface_commit,
};

struct wl_listener buffer_destroy_listener = {
	.notify = on_buffer_destroy,
};

int main() {
	signal(SIGINT, on_interrupt);

	// Creating display
	display = wl_display_create();
	if (!display) {
		fprintf(stderr, "ERROR: Couldn't create Wayland display.\n");
		return 1;
	}

	// Adding socket to display
	const char* socket = wl_display_add_socket_auto(display);
	if (!socket) {
		fprintf(stderr, "ERROR: Couldn't add socket to Wayland display.\n");
		return 1;
	}

	// Shared Memory
	wl_display_init_shm(display);
	for (int i = 0; i < (int)SHM_FORMATS_SIZE; ++i) {
		wl_display_add_shm_format(display, shm_formats[i]);
	}

	// Raylib
	SetTraceLogLevel(LOG_WARNING); /* getting rid of annoying init info */
	InitWindow(RL_DEFAULT_WIDTH, RL_DEFAULT_HEIGHT, RL_TITLE);
	//

	// Listen to commit and buffer destroy from surfaces (to draw buffers)
	surface_commit_initialize();
	surface_commit_add_listener(&surface_commit_listener);
	buffer_destroy_initialize();
	buffer_destroy_add_listener(&buffer_destroy_listener);

	// Creating Compositor
	if (compositor_new(display)) {
		return_value = 1;
		goto end;
	}
	//

	printf("Running Wayland display on socket %s...\n", socket);
	wl_display_run(display); // Blocking until wl_display_terminate

end:
	wl_display_destroy(display);
	if (return_value) printf("An error occured :(\n");
	else printf("Done :3\n");
	return return_value;
}
