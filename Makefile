CC = g++

SRC_DIR = src/

PATHTRACER_FILES = $(SRC_DIR)/utils/vector3.cpp \
		$(SRC_DIR)/utils/ray.cpp \
		$(SRC_DIR)/objects/texture.cpp \
		$(SRC_DIR)/objects/models/sphere.cpp \
		$(SRC_DIR)/objects/models/square.cpp \
		$(SRC_DIR)/objects/models/triangle_mesh.cpp \

		#$(SRC_DIR)/objects/object.cpp\
		#$(SRC_DIR)/objects/sphere.cpp\
		#$(SRC_DIR)/objects/square.cpp\
		#$(SRC_DIR)/objects/triangle_mesh.cpp\

PATHTRACER_H_FILES = $(SRC_DIR)/utils/vector3.hh \
		$(SRC_DIR)/objects/texture.hh\
		$(SRC_DIR)/objects/material.hh\

CPP_FILES = $(RAYTRACER_FILES) $(PATHTRACER_FILES)
H_FILES = $(RAYTRACER_H_FILES) $(PATHTRACER_H_FILES)



OBJ_FILES = $(CPP_FILES:.cpp=.o)


CXX_FLAGS += -Wall -Wextra -O3 -g -std=c++11 -pthread -faligned-new \
		-fsanitize=address	\
		-I$(SRC_DIR)

CXX_FLAGS += -lm

BIN = main

%.o: %.c $(H_FILES)
	$(CC) -c -o $@ $< $(CFLAGS)

all: build

build: $(OBJ_FILES)
	$(CC)  $(SRC_DIR)/main.cpp -o $(BIN) $(OBJ_FILES) $(CXX_FLAGS)

.PHONY: all clean build

clean:
	rm -f $(OBJ_FILES) $(BIN)
