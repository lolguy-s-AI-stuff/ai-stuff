UNAME_S = $(shell uname -s)

CC = clang++
CFLAGS = -O2 -g -Wall -Wextra -Wpedantic -Wstrict-aliasing -Wc++11-narrowing -Wvla-extension
CFLAGS += -Wno-pointer-arith -Wno-newline-eof -Wno-unused-parameter -Wno-gnu-statement-expression
CFLAGS += -Wno-gnu-compound-literal-initializer -Wno-gnu-zero-variadic-macro-arguments
CFLAGS += -Ilib/glad/include -Ilib/glfw/include -Ilib/libcai/include -Isrc -Isrc/vendor/  
LDFLAGS = lib/glad/src/glad.o lib/libcai/bin/libcai.a lib/glfw/src/libglfw3.a -ldl -lpthread -lGL



SRC  = $(wildcard src/*.cpp) $(wildcard src/**/*.cpp) $(wildcard src/*.cpp) $(wildcard src/**/**/*.cpp) $(wildcard src/**/**/**/*.cpp)
OBJ  = $(SRC:.cpp=.o)
BIN  = ./bin


.PHONY: all clean

all: dirs libs game

libs:
	cd lib/glad && $(CC) -o src/glad.o -Iinclude -c src/glad.c
	cd lib/libcai && make 
	cd lib/glfw && cmake . && make

dirs:
	mkdir -p ./$(BIN)

run: all
	$(BIN)/game

game: $(OBJ)
	$(CC) -o $(BIN)/game $^ $(LDFLAGS)

%.o: %.cpp
	$(CC) -o $@ -c $< $(CFLAGS)

clean:
	rm -rf $(BIN) $(OBJ)