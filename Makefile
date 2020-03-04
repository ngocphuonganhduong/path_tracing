CC = g++
RAYTRACER_DIR = src/raytracer
SRC_DIR = src/
PATHTRACER_DIR = src/pathtracer

RAYTRACER_FILES = $(RAYTRACER_DIR)/vector3.cpp \
		$(RAYTRACER_DIR)/ray.cpp \
		$(RAYTRACER_DIR)/objects/texture.cpp \
		$(RAYTRACER_DIR)/objects/object.cpp\
		$(RAYTRACER_DIR)/objects/sphere.cpp\
		$(RAYTRACER_DIR)/objects/square.cpp\
		$(RAYTRACER_DIR)/objects/triangle_mesh.cpp\
		$(RAYTRACER_DIR)/lights/ambient.cpp \
		$(RAYTRACER_DIR)/lights/directional.cpp \
		$(RAYTRACER_DIR)/lights/point.cpp \
		$(RAYTRACER_DIR)/scene.cpp\
		$(RAYTRACER_DIR)/raytracer.cpp

RAYTRACER_H_FILES =  $(RAYTRACER_DIR)/vector3.hh \
		$(RAYTRACER_DIR)/ray.hh\
		$(RAYTRACER_DIR)/objects/texture.hh\
		$(RAYTRACER_DIR)/objects/object.hh \
		$(RAYTRACER_DIR)/lights/light.hh\
		$(RAYTRACER_DIR)/scene.hh \
		$(RAYTRACER_DIR)/raytracer.hh \


PATHTRACER_FILES = $(PATHTRACER_DIR)/utils/vector3.cpp \
		$(PATHTRACER_DIR)/objects/material.cpp \
		#$(PATHTRACER_DIR)/objects/object.cpp\
		#$(PATHTRACER_DIR)/objects/sphere.cpp\
		#$(PATHTRACER_DIR)/objects/square.cpp\
		#$(PATHTRACER_DIR)/objects/triangle_mesh.cpp\

PATHTRACER_H_FILES = $(PATHTRACER_DIR)/utils/vector3.hh \
		$(PATHTRACER_DIR)/objects/material.hh\

CPP_FILES = $(RAYTRACER_FILES) $(PATHTRACER_FILES)
H_FILES = $(RAYTRACER_H_FILES) $(PATHTRACER_H_FILES)



OBJ_FILES = $(CPP_FILES:.cpp=.o)


CXX_FLAGS += -Wall -Wextra -O3 -g -std=c++11 -pthread -faligned-new \
		-fsanitize=address	\
		-I$(RAYTRACER_DIR)	\
		-I$(CR_DIR)		\
		-I$(SRC_DIR)

CXX_FLAGS += -lm

BIN = main
BIN_RT = rt

%.o: %.c $(H_FILES)
	$(CC) -c -o $@ $< $(CFLAGS)

all: build

build: $(OBJ_FILES)
	$(CC)  $(SRC_DIR)/main.cpp -o $(BIN) $(OBJ_FILES) $(CXX_FLAGS)

#For Raytracer testing
rt: $(OBJ_FILES)
	$(CC) $(SRC_DIR)/rt.cpp -o $(BIN_RT) $(OBJ_FILES) $(CXX_FLAGS)

.PHONY: all clean build

clean:
	rm -f $(OBJ_FILES) $(BIN) $(BIN_RT)
