CC = g++
RAYTRACER_DIR = src/raytracer
SRC_DIR = src/

CPP_FILES = $(RAYTRACER_DIR)/vector3.cpp \
		$(RAYTRACER_DIR)/ray.cpp \
		$(RAYTRACER_DIR)/objects/texture.cpp \
		$(RAYTRACER_DIR)/objects/object.cpp\
		$(RAYTRACER_DIR)/objects/sphere.cpp\
		$(RAYTRACER_DIR)/objects/square.cpp\
		$(RAYTRACER_DIR)/lights/ambient.cpp \
		$(RAYTRACER_DIR)/lights/directional.cpp \
		$(RAYTRACER_DIR)/lights/point.cpp \
		$(RAYTRACER_DIR)/scene.cpp\
		$(RAYTRACER_DIR)/raytracer.cpp

H_FILES =  $(RAYTRACER_DIR)/vector3.hh \
		$(RAYTRACER_DIR)/ray.hh\
		$(RAYTRACER_DIR)/objects/texture.hh\
		$(RAYTRACER_DIR)/objects/object.hh \
		$(RAYTRACER_DIR)/lights/light.hh\
		$(RAYTRACER_DIR)/scene.hh \
		$(RAYTRACER_DIR)/raytracer.hh \



OBJ_FILES = $(CPP_FILES:.cpp=.o)


CXX_FLAGS += -Wall -Wextra -O3 -g -std=c++11 -pthread -faligned-new \
		-fsanitize=address		\
		-I$(RAYTRACER_DIR)		\
		-I$(SRC_DIR)

CXX_FLAGS += -lm
#CXX_FLAGS += -m64 -march=native
#CXX_FLAGS += -fopt-info-vec-optimized #-fopt-info-vec-missed -ftree-vectorize

BIN = tp1
TEST = test

%.o: %.c $(H_FILES)
	$(CC) -c -o $@ $< $(CFLAGS)

all: build

build: $(OBJ_FILES)
	$(CC)  tp1.cc -o $(BIN) $(OBJ_FILES) $(CXX_FLAGS)

test: $(OBJ_FILES)
	$(CC) test.cc -o $(TEST) $(OBJ_FILES) $(CXX_FLAGS)

.PHONY: all clean build

clean:
	rm -f $(OBJ_FILES) $(BIN) $(TEST)
