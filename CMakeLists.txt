cmake_minimum_required(VERSION 3.15)
project(pathtracer)
set(CMAKE_CXX_STANDARD 17)

set(SOURCE
        src/utils/vector3.hh
        src/utils/ray.hh
        src/utils/sampler.cpp
        src/utils/sampler.hh
        src/objects/bsdf/bsdf.hh
        src/objects/bsdf/bsdf.cpp
        src/objects/bsdf/diffuse.cpp
        src/objects/bsdf/mirror.cpp
        src/objects/bsdf/phong.cpp
        src/objects/bsdf/blinn_phong.cpp
        src/objects/material.hh
        src/objects/material.cpp
        src/objects/object.cpp
        src/objects/object.hh
        src/objects/models/sphere.cpp
        src/objects/models/square.cpp
        src/objects/models/triangle_mesh.cpp
        src/objects/object.cpp
        src/objects/object_loader.cpp
        src/scene.cpp
        src/render.cpp
        src/classic_pt.cpp
        src/pt.cpp
        src/parser/parse_arg.cpp
        src/bdpt.cpp
        src/bdpt_generate_path.cpp
        )
find_package (Threads)
set(CMAKE_XCODE_GENERATE_SCHEME ON)
set(CMAKE_XCODE_SCHEME_ADDRESS_SANITIZER ON)
set(CMAKE_XCODE_SCHEME_ADDRESS_SANITIZER_USE_AFTER_RETURN ON)
add_executable(main src/main.cpp ${SOURCE})

add_compile_options(-Wall -Werror -Wextra -fsanitize=address)
target_link_libraries (main ${CMAKE_THREAD_LIBS_INIT} -fsanitize=address)
