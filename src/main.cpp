#include "pathtracer.hh"
#include <cstring>

using namespace pathtracing;

int main(int argc, char **argv) {
    unsigned int nb_samples = 4;
    //Debug ray declaration
    unsigned x = 0;
    unsigned y = 0;
    for (int i = 1; i < argc; ++i)
    {
        if (strcmp(argv[i], "--debug") == 0) {
            debug = true;
        }
        else if (strcmp(argv[i], "-ns") == 0){
            ++i;
            nb_samples = std::atoi(argv[i]);
        }
        else if (debug && strcmp(argv[i], "--ray") == 0)
        {
            debug_ray = true;
            x = std::atoi(argv[i + 1]);
            y = std::atoi(argv[i + 2]);
            i += 2;
        }
    }
    //TEXTURE
    shared_text red = std::make_shared<UniformTexture>(Vector3(1,0,0));
    shared_text green = std::make_shared<UniformTexture>(Vector3(0,1,0));
    shared_text blue = std::make_shared<UniformTexture>(Vector3(0,0,1));
    shared_text white = std::make_shared<UniformTexture>(Vector3(1,1,1));

    shared_text blue_white = std::make_shared<CheckedTexture>(Vector3(0,0,1),
                                                              Vector3(1,1,1));
    //MODEL
    shared_mod sp_small = std::make_shared<Sphere>(0.5);
    shared_mod sp_med = std::make_shared<Sphere>(1.0);
    TriVector tv;
    shared_mod trimesh = std::make_shared<TriangleMesh>(tv, Vector3(1,1,1));

    float half_l = 4;
    shared_mod f_wallm = std::make_shared<Square>(Vector3(0, -1, 0),
                                                     Vector3(0, 0, 1),
                                                     half_l);
    shared_mod lr_wallm = std::make_shared<Square>(Vector3(1, 0, 0),
                                                  Vector3(0, 0, 1),
                                                  half_l);
    shared_mod tb_wallm = std::make_shared<Square>(Vector3(0, 0, -1),
                                                  Vector3(0, 1, 0),
                                                  half_l);

    //MATERIAL
    float s = 32; //shininess;
    float ka = 1;
    float ks = 0.5;
    float kd = 0.5;
    shared_mat mat1 = std::make_shared<Material>(ka, kd, ks, s, 1,
                                                  Vector3(1,1,1));

    shared_mat mat2 = std::make_shared<Material>(0, 0, 0, 0, 0,//light source
                                                 Vector3(1,1,1));
    //OBJECT
    shared_obj o1 = std::make_shared<Object>(Vector3(0,14,0), mat1, red, sp_med);

    //LIGHT OBJECT
    shared_obj o2 = std::make_shared<Object>(Vector3(-1,8,1), Vector3(1,1,1),
                                             mat1, white, sp_small); //light source

    //WALL
    float distance = 16;
    shared_obj front_w = std::make_shared<Object>(Vector3(0, distance,0),
                                                  mat1, blue, f_wallm); //light source

    shared_obj roof = std::make_shared<Object>(Vector3(0, distance - half_l,
                                                       half_l),
                                               mat1, blue, tb_wallm); //light source
    shared_obj floor = std::make_shared<Object>(Vector3(0, distance - half_l,
                                                        - half_l),
                                                mat1, blue, tb_wallm); //light source
    shared_obj left_w = std::make_shared<Object>(Vector3(-half_l, distance - half_l, 0),
                                                 mat1, blue, lr_wallm); //light source

    shared_obj right_w = std::make_shared<Object>(Vector3(half_l, distance - half_l, 0),
                                                  mat1, blue, lr_wallm); //light source
    Camera cam(Vector3(0,0,0), Vector3(0,1,0), Vector3(0,0,1), 60);
    int max_iter = 10;
    Scene scene(500, 400, cam, max_iter, Vector3(0.1, 0.1, 0.1));
    scene.add_object(o1);
    scene.add_object(o2);
    scene.add_object(front_w);
    scene.add_object(roof);
    scene.add_object(floor);
    scene.add_object(left_w);
    scene.add_object(right_w);

    Pathtracer pt(scene, nb_samples);
    if (debug_ray)
    {
        std::cout << pt.trace(x, y) << "\n";
    }
    else {
        pt.render();
        pt.to_ppm("output.ppm");
    }
    return 0;
}
