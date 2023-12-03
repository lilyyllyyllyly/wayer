# Compiler stuffs
COMP=clang
OPTIONS=-Wall -Wextra -Werror -Wno-unused-parameter -Wno-unused-variable -O3
DEBUG=-fsanitize=address,undefined -g3
OUTPUT=server
# ---

#XDG_SHELL_PATH=/usr/share/wayland-protocols/stable/xdg-shell/xdg-shell.xml
SOURCES=server.c compositor.c surface.c

final:
	$(COMP) $(OPTIONS) -lwayland-server -lraylib $(SOURCES) -o $(OUTPUT)

debug:
	$(COMP) $(OPTIONS) $(DEBUG) -lwayland-server -lraylib $(SOURCES) -o $(OUTPUT)

#xdg:
#	wayland-scanner private-code $(XDG_SHELL_PATH) xdg-shell-protocol.c
#	wayland-scanner server-header $(XDG_SHELL_PATH) xdg-shell-server-protocol.h
