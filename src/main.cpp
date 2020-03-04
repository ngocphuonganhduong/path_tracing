#include <stdio.h>
#include "pathtracer.hh"

using namespace pathtracing;
int main() {
    //TEXTURE
    shared_text red = std::make_shared<UniformTexture>(Vector3(1,0,0));
    shared_text green = std::make_shared<UniformTexture>(Vector3(0,1,0));
    shared_text blue = std::make_shared<UniformTexture>(Vector3(0,0,1));
    shared_text white = std::make_shared<UniformTexture>(Vector3(1,1,1));

    shared_text blue_white = std::make_shared<CheckedTexture>(Vector3(0,0,1),
                                                              Vector3(1,1,1));
    //MODEL
    shared_mod sphere = std::make_shared<Sphere>(2.0);
    TriVector tv;
    shared_mod trimesh = std::make_shared<TriangleMesh>(tv, Vector3(1,1,1));
    shared_mod square = std::make_shared<Square>(Vector3(1,1,1), Vector3(1,1,0),
                                                 4);

    //MATERIAL
    float s = 32; //shininess;
    float ka = 1;
    float ks = 0.9;
    float kd = 0.8;
    shared_mat mat1 = std::make_shared<Material>(ka, kd, ks, s, 1,
                                                  Vector3(1,1,1));

    shared_mat mat2 = std::make_shared<Material>(0, 1, 0, 0, 0,
                                                 Vector3(1,1,1));
    //OBJECT
    shared_obj o1 = std::make_shared<Object>(Vector3(0,0,5), Vector3(0,0,0),
                                             mat1, red, sphere);
    shared_obj o2 = std::make_shared<Object>(Vector3(0,0,5), Vector3(1,1,1),
                                             mat1, white, sphere); //light source

    return 0;
}
