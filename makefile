# Compiler stuffs
COMP=clang
OPTIONS=-Wall -Wextra -Werror -Wno-unused-parameter -O3
DEBUG=-fsanitize=address,undefined -g3
OUTPUT=server
# ---

XDG_SHELL_PATH=/usr/share/wayland-protocols/stable/xdg-shell/xdg-shell.xml
SOURCES=server.c compositor.c xdg-shell-protocol.c

final: xdg
	$(COMP) $(OPTIONS) -lwayland-server $(SOURCES) -o $(OUTPUT)

debug: xdg
	$(COMP) $(OPTIONS) $(DEBUG) -lwayland-server $(SOURCES) -o $(OUTPUT)

xdg:
	wayland-scanner private-code $(XDG_SHELL_PATH) xdg-shell-protocol.c
	wayland-scanner server-header $(XDG_SHELL_PATH) xdg-shell-server-protocol.h
