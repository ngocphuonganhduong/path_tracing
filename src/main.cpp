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
        if (strcmp(argv[i], "--debug") == 0 || strcmp(argv[i], "-d") == 0) {
            debug = true;
        }
        else if (strcmp(argv[i], "-ns") == 0){
            ++i;
            nb_samples = std::atoi(argv[i]);
        }
        else if (debug && (strcmp(argv[i], "--ray") == 0
                           || strcmp(argv[i], "-r") == 0))
        {
            debug_ray = true;
            x = std::atoi(argv[i + 1]);
            y = std::atoi(argv[i + 2]);
            i += 2;
        }
    }
    //color
    Vector3 red(1,0,0);
    Vector3 green(0,1,0);
    Vector3 blue(0,0,1);
    Vector3 pink(1, 0, 0.75);
    Vector3 white(1,1,1);

    //TEXTURE
    shared_text text1 = std::make_shared<UniformTexture>(Vector3(1,0.85,0.75));

    shared_text blue_white = std::make_shared<CheckedTexture>(Vector3(0,0,1),
                                                              Vector3(1,1,1));
    //MODEL
    shared_mod sp_small = std::make_shared<Sphere>(0.4);
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
    float ns = 32; //shininess;
    Vector3 ka(0.1, 0.1, 0.1); //ambient coef
    Vector3 ks(1,1,1); //specular coef
    Vector3 kd(0.8, 0.8, 0.8); //diffuse coef

    //MATERIAL
    shared_mat mat1 = std::make_shared<Material>(ka, kd, ks, ns);

    shared_mat mat3 = std::make_shared<Material>(ka, kd, ks, ns);


    //MATERIAL for light object
    shared_mat mat2 = std::make_shared<Material>(ka, kd, ks, ns);
    shared_mat mat4 = std::make_shared<Material>(ka, kd, ks, ns, 0.1, 0.2, 1);

    //OBJECT
    shared_obj o1 = std::make_shared<Object>(Vector3(-1.5,13,-3), mat1, red,
                                             sp_med);

    //LIGHT OBJECT
    shared_obj o2 = std::make_shared<Object>(Vector3(0,12,3.75), Vector3(0.9,0.9,0.9),
                                             mat2, white, sp_small); //light source
    shared_obj o3 = std::make_shared<Object>(Vector3(2,10,-3), pink,
                                             mat4, pink, sp_small); //light source

    //WALL
    float distance = 16;
    shared_obj front_w = std::make_shared<Object>(Vector3(0, distance,0),
                                                  mat3, text1, f_wallm);
    shared_obj roof = std::make_shared<Object>(Vector3(0, distance - half_l,
                                                       half_l),
                                               mat3, blue, tb_wallm);
    shared_obj floor = std::make_shared<Object>(Vector3(0, distance - half_l,
                                                        - half_l),
                                                mat1, blue_white, tb_wallm);
    shared_obj left_w = std::make_shared<Object>(Vector3(-half_l, distance - half_l, 0),
                                                 mat1, text1, lr_wallm);
    shared_obj right_w = std::make_shared<Object>(Vector3(half_l, distance - half_l, 0),
                                                  mat1, text1, lr_wallm);
    Camera cam(Vector3(0,0,0), Vector3(0,1,0), Vector3(0,0,1), 60);
    int max_iter = 10;
    Scene scene(500, 450, cam, max_iter, Vector3(0.1, 0.1, 0.1));
    scene.add_object(o1);
    scene.add_object(o2);
    scene.add_object(o3);
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
