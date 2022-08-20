CC = clang
CFLAGS = -std=gnu17 -O2 -Wall -Wextra -Wpedantic -Wstrict-aliasing
CFLAGS += -Wno-pointer-arith -Wno-newline-eof -Wno-unused-parameter -Wno-gnu-statement-expression -Wno-unused-variable -Wno-unused-function
CFLAGS += -Wno-gnu-compound-literal-initializer -Wno-gnu-zero-variadic-macro-arguments
CFLAGS += -Isrc -Ilib/cglm/include -Ilib/glad/include -Ilib/glfw/include `pkg-config --cflags glib-2.0` -Ilib/stb
LDFLAGS = lib/cglm/libcglm.a lib/glad/src/glad.o lib/glfw/src/libglfw3.a -lm `pkg-config --libs glib-2.0`
LDFLAGS += -lX11 -ldl -lpthread

SRC  = $(wildcard src/**/*.c) $(wildcard src/*.c) $(wildcard src/**/**/*.c) $(wildcard src/**/**/**/*.c)
OBJ  = $(SRC:.c=.o)
BIN  = bin

.PHONY: all clean

all: dirs libs game

dirs:
	mkdir -p ./$(BIN)

libs:
	cd lib/cglm && cmake . -DCGLM_STATIC=ON && make
	cd lib/glad && $(CC) -o src/glad.o -Iinclude -c src/glad.c
	cd lib/glfw && cmake . && make

run: all
	$(BIN)/game

game: $(OBJ)
	$(CC) -o $(BIN)/game $^ $(LDFLAGS)

%.o: %.c
	$(CC) -o $@ -c $< $(CFLAGS)

clean:
	rm -rf $(BIN) $(OBJ)
