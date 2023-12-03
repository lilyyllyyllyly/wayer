# Compiler stuffs
COMP=clang
OPTIONS=-Wall -Wextra -Werror -Wno-unused-parameter -Wno-unused-variable -O3
DEBUG=-fsanitize=address,undefined -g3
OUTPUT=server
# ---

SOURCES=server.c compositor.c surface.c

final:
	$(COMP) $(OPTIONS) -lwayland-server -lraylib $(SOURCES) -o $(OUTPUT)

debug:
	$(COMP) $(OPTIONS) $(DEBUG) -lwayland-server -lraylib $(SOURCES) -o $(OUTPUT)
