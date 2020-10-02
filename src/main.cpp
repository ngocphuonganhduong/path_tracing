#include <cstring>
#include "parser/parser.hh"
#include "objects/object_loader.hh"

using namespace pathtracing;
std::unordered_map<std::string, Vector3> cols = {
        {"red",    {1,    0.4,  0.4}},
        {"green",  {0.4,  1,    0.4}},
        {"blue",   {0.4,  0.4,  1}},
        {"pink",   {1,    0.6,  0.8}},
        {"white",  {1,    1,    1}},
        {"black",  {0,    0,    0}},
        {"yellow", {1,    1,    0.2}},
        {"cyan",   {0,    1,    1}},
        {"purple", {0.9,  0.3,  1}},
        {"wall",   {0.95, 0.92, 0.97}},
};

Scene simple() {
    double half_l = 4;
    double distance = 16;

    //MATERIAL
    Vector3 ka(0.1, 0.1, 0.1);
    Vector3 ks(1, 1, 1); //specular coef
    double ns = 32; //shininess;

    shared_mat mat = std::make_shared<Material>(ka, cols["pink"], cols["white"], 32);
    auto o1 = std::make_shared<Sphere>(Vector3(1.5, 11, -2.5), std::make_shared<DiffuseBSDF>(mat), 1.4);

    shared_mat mat2 = std::make_shared<Material>(ka, cols["yellow"], cols["black"], 32);
    auto o2 = std::make_shared<Sphere>(Vector3(-2, 10, -3), std::make_shared<DiffuseBSDF>(mat2), 1);

    shared_mat mat_light = std::make_shared<Material>(ka, cols["white"], ks, ns);
    mat_light->set_emission(cols["white"] * 7, 0.4, 0, 1);


    shared_obj area_light = std::make_shared<Square>(Vector3(0, distance - 4, half_l - 0.1),
                                                     std::make_shared<DiffuseBSDF>(mat_light), Vector3(0, 0, -1),
                                                     Vector3(0, 1, 0),
                                                     1.5);

    //WALLVector3(1, 0.6, 0.15)
    shared_mat mat_f = std::make_shared<Material>(ka, cols["green"], ks, 32);
    shared_obj floor = std::make_shared<Square>(Vector3(0, distance - half_l,
                                                        -half_l),
                                                std::make_shared<DiffuseBSDF>(mat_f),
                                                Vector3(0, 0, 1),
                                                Vector3(0, 1, 0),
                                                half_l);


    shared_bsdf wall_bsdf = std::make_shared<DiffuseBSDF>(std::make_shared<Material>(ka, cols["white"], ks, ns));
    shared_bsdf root_bsdf = std::make_shared<DiffuseBSDF>(std::make_shared<Material>(ka, cols["wall"], ks, ns));
    shared_bsdf left_wall_bsdf = std::make_shared<DiffuseBSDF>(std::make_shared<Material>(ka, cols["red"], ks, ns));
    shared_bsdf right_wall_bsdf = std::make_shared<DiffuseBSDF>(std::make_shared<Material>(ka, cols["blue"], ks, ns));


    shared_obj roof = std::make_shared<Square>(Vector3(0, distance - half_l, half_l),
                                               root_bsdf, Vector3(0, 0, -1),
                                               Vector3(0, 1, 0),
                                               half_l);

    shared_obj front_w = std::make_shared<Square>(Vector3(0, distance, 0),
                                                  wall_bsdf, Vector3(0, -1, 0),
                                                  Vector3(0, 0, 1),
                                                  half_l);


    shared_obj left_w = std::make_shared<Square>(Vector3(-half_l, distance - half_l, 0),
                                                 left_wall_bsdf, Vector3(1, 0, 0),
                                                 Vector3(0, 0, 1),
                                                 half_l);

    shared_obj right_w = std::make_shared<Square>(Vector3(half_l, distance - half_l, 0),
                                                  right_wall_bsdf, Vector3(-1, 0, 0),
                                                  Vector3(0, 0, 1),
                                                  half_l);
    Camera cam(Vector3(0, 0, 0), Vector3(0, 1, 0), Vector3(0, 0, 1), 60);
    Scene scene(500, 450, cam, Vector3(0.2, 0.2, 0.2));

    scene.add_object(o1);
    scene.add_object(o2);
    scene.add_object(area_light);
    scene.add_object(front_w);
    scene.add_object(roof);
    scene.add_object(floor);
    scene.add_object(left_w);
    scene.add_object(right_w);
    return scene;
}

Scene cornell_box() {
    double half_l = 4;
    double distance = 16;

    objl::Loader loader;
    TriVector t;
    TriVector t2;

    loader.load_object("../src/objects/assets/box.obj", t);
    loader.load_object("../src/objects/assets/tall_box.obj", t2);
    //MATERIAL
    Vector3 ka(0.2, 0.2, 0.2);
    Vector3 ks(1, 1, 1); //specular coef
    double ns = 32; //shininess;

    shared_mat mat = std::make_shared<Material>(ka, cols["white"], cols["white"], 32);
    auto o1 = std::make_shared<TriangleMesh>(Vector3(2, 11, -3.4), std::make_shared<DiffuseBSDF>(mat), t,
                                             Vector3(1.2, 1.2, 1.2), Vector3(0, 0, -30));
    auto o2 = std::make_shared<TriangleMesh>(Vector3(-2, 12.5, -2.6), std::make_shared<DiffuseBSDF>(mat), t2,
                                             Vector3(1.2, 1.2, 1.2), Vector3(0, 0, 30));
    auto o3 = std::make_shared<Sphere>(Vector3(-2, 12.5, 0.94), std::make_shared<MirrorBSDF>(mat), 1.0);
    shared_mat mat_light = std::make_shared<Material>(ka, cols["white"], ks, ns);
    mat_light->set_emission(cols["white"] * 5, 0.2, 0, 1);


    shared_obj area_light = std::make_shared<Square>(Vector3(0, distance - 4, half_l - 0.1),
                                                     std::make_shared<DiffuseBSDF>(mat_light), Vector3(0, 0, -1),
                                                     Vector3(0, 1, 0),
                                                     1.5);

    //WALL
    shared_mat mat_f = std::make_shared<Material>(ka, cols["green"], cols["white"], 100);
    shared_obj floor = std::make_shared<Square>(Vector3(0, distance - half_l,
                                                        -half_l),
                                                std::make_shared<PhongBSDF>(mat_f),
                                                Vector3(0, 0, 1),
                                                Vector3(0, 1, 0),
                                                half_l * 4);


    shared_bsdf wall_bsdf = std::make_shared<DiffuseBSDF>(std::make_shared<Material>(ka, cols["white"], ks, ns));
    shared_bsdf root_bsdf = std::make_shared<DiffuseBSDF>(std::make_shared<Material>(ka, cols["white"], ks, ns));
    shared_bsdf left_wall_bsdf = std::make_shared<DiffuseBSDF>(std::make_shared<Material>(ka, cols["red"], ks, ns));
    shared_bsdf right_wall_bsdf = std::make_shared<DiffuseBSDF>(std::make_shared<Material>(ka, cols["blue"], ks, ns));
    shared_obj roof = std::make_shared<Square>(Vector3(0, distance - half_l, half_l),
                                               root_bsdf, Vector3(0, 0, -1),
                                               Vector3(0, 1, 0),
                                               half_l * 4);

    shared_obj front_w = std::make_shared<Square>(Vector3(0, distance, 0),
                                                  wall_bsdf, Vector3(0, -1, 0),
                                                  Vector3(0, 0, 1),
                                                  half_l * 4);
    shared_obj behind_w = std::make_shared<Square>(Vector3(0, -0.01, 0),
                                                   wall_bsdf, Vector3(0, 1, 0),
                                                   Vector3(0, 0, 1),
                                                   half_l * 1000);


    shared_obj left_w = std::make_shared<Square>(Vector3(-half_l, distance - half_l, 0),
                                                 left_wall_bsdf, Vector3(1, 0, 0),
                                                 Vector3(0, 0, 1),
                                                 half_l * 4);

    shared_obj right_w = std::make_shared<Square>(Vector3(half_l, distance - half_l, 0),
                                                  right_wall_bsdf, Vector3(-1, 0, 0),
                                                  Vector3(0, 0, 1),
                                                  half_l * 4);
    Camera cam(Vector3(0, 0, 0), Vector3(0, 1, 0), Vector3(0, 0, 1), 60);
    Scene scene(400, 400, cam, Vector3(0.2, 0.2, 0.2));

    scene.add_object(o1);
    scene.add_object(o2);
    scene.add_object(o3);
    scene.add_object(area_light);
    scene.add_object(front_w);
    scene.add_object(behind_w);
    scene.add_object(roof);
    scene.add_object(floor);
    scene.add_object(left_w);
    scene.add_object(right_w);
    return scene;
}

Scene multi_light() {
    double half_l = 4.6;
    double distance = 16;

    //MATERIAL
    Vector3 ka(0.1, 0.1, 0.1);
    Vector3 ks(1, 1, 1); //specular coef
    double ns = 32; //shininess;
    double ni = 1;
    double kr = 0.5;
    double d = 1;

    //OBJECT
    //type, ka, kd, ks, ns, ni, d
    shared_mat mat = std::make_shared<Material>(ka, cols["pink"] * 0.5, cols["white"], 100);
    auto o1 = std::make_shared<Sphere>(Vector3(0, distance - half_l, -2.8), std::make_shared<MirrorBSDF>(mat), 1.6);

    Vector3 orange(0.7, 0.35, 0);
    shared_mat mat2 = std::make_shared<Material>(ka, orange, cols["white"], 32);
    mat2->set_emission(orange, 0.3, 0, 1.0);
    auto o2 = std::make_shared<Sphere>(Vector3(-2.5, 12.5, -4.5), std::make_shared<PhongBSDF>(mat2), 0.5);

    Vector3 yellow(0.7, 0.7, 0.0);
    shared_mat mat3 = std::make_shared<Material>(ka, yellow, cols["white"], 32);
    mat3->set_emission(yellow, 0.3, 0, 1.0);
    auto o3 = std::make_shared<Sphere>(Vector3(-1.5, 10.5, -4.5), std::make_shared<PhongBSDF>(mat3), 0.5);

    Vector3 green2(0, 0.7, 0);
    shared_mat mat4 = std::make_shared<Material>(ka, green2, cols["white"], 32);
    mat4->set_emission(green2, 0.3, 0, 1.0);
    auto o4 = std::make_shared<Sphere>(Vector3(0, 9.5, -4.5), std::make_shared<PhongBSDF>(mat4), 0.5);

    Vector3 blue2(0, 0, 0.7);
    shared_mat mat5 = std::make_shared<Material>(ka, blue2, cols["white"], 32);
    mat5->set_emission(blue2, 0.3, 0, 1.0);
    auto o5 = std::make_shared<Sphere>(Vector3(1.5, 10.5, -4.5), std::make_shared<PhongBSDF>(mat5), 0.5);

    Vector3 purple(0.4, 0, 0.7);
    shared_mat mat6 = std::make_shared<Material>(ka, purple, cols["white"], 32);
    mat6->set_emission(purple, 0.3, 0, 1.0);
    auto o6 = std::make_shared<Sphere>(Vector3(2.5, 12.5, -4.5), std::make_shared<PhongBSDF>(mat6), 0.5);


    shared_mat mat_light = std::make_shared<Material>(ka, cols["black"], ks, ns);
    mat_light->set_emission(cols["white"] * 5, 0.3, 0, 1);

    shared_obj area_light = std::make_shared<Square>(Vector3(0, distance - 4, half_l - 0.1),
                                                     std::make_shared<DiffuseBSDF>(mat_light), Vector3(0, 0, -1),
                                                     Vector3(0, 1, 0),
                                                     1.5);

    shared_mat mat_f = std::make_shared<Material>(ka, cols["white"], ks, 32);
    shared_obj floor = std::make_shared<Square>(Vector3(0, distance - half_l,
                                                        -half_l),
                                                std::make_shared<DiffuseBSDF>(mat_f),
                                                Vector3(0, 0, 1),
                                                Vector3(0, 1, 0),
                                                half_l);


    shared_bsdf wall_bsdf = std::make_shared<DiffuseBSDF>(std::make_shared<Material>(ka, cols["white"], ks, ns));
    shared_bsdf root_bsdf = std::make_shared<DiffuseBSDF>(std::make_shared<Material>(ka, cols["white"], ks, ns));
    shared_bsdf left_wall_bsdf = std::make_shared<DiffuseBSDF>(std::make_shared<Material>(ka, cols["red"], ks, ns));
    shared_bsdf right_wall_bsdf = std::make_shared<DiffuseBSDF>(std::make_shared<Material>(ka, cols["blue"], ks, ns));

    shared_obj roof = std::make_shared<Square>(Vector3(0, distance - half_l, half_l),
                                               root_bsdf, Vector3(0, 0, -1),
                                               Vector3(0, 1, 0),
                                               half_l * 4);

    shared_obj front_w = std::make_shared<Square>(Vector3(0, distance, 0),
                                                  wall_bsdf, Vector3(0, -1, 0),
                                                  Vector3(0, 0, 1),
                                                  half_l * 4);
    shared_obj behind_w = std::make_shared<Square>(Vector3(0, -0.01, 0),
                                                   wall_bsdf, Vector3(0, 1, 0),
                                                   Vector3(0, 0, 1),
                                                   half_l * 1000);


    shared_obj left_w = std::make_shared<Square>(Vector3(-half_l, distance - half_l, 0),
                                                 left_wall_bsdf, Vector3(1, 0, 0),
                                                 Vector3(0, 0, 1),
                                                 half_l * 4);

    shared_obj right_w = std::make_shared<Square>(Vector3(half_l, distance - half_l, 0),
                                                  right_wall_bsdf, Vector3(-1, 0, 0),
                                                  Vector3(0, 0, 1),
                                                  half_l * 4);
    Camera cam(Vector3(0, 0, 0), Vector3(0, 1, 0), Vector3(0, 0, 1), 60);
    Scene scene(400, 400, cam, Vector3(0.2, 0.2, 0.2));

    scene.add_object(o1);
    scene.add_object(o2);
    scene.add_object(o3);
    scene.add_object(o4);
    scene.add_object(o5);
    scene.add_object(o6);

    scene.add_object(area_light);
    scene.add_object(front_w);
    scene.add_object(behind_w);
    scene.add_object(roof);
    scene.add_object(floor);
    scene.add_object(left_w);
    scene.add_object(right_w);
    return scene;
}

Scene pyramids() {
    objl::Loader loader;
    TriVector t1;
    TriVector t2;
    TriVector t3;
    loader.load_object("../src/objects/assets/tetrahedron.obj", t1);
    loader.load_object("../src/objects/assets/tetrahedron.obj", t2);
    loader.load_object("../src/objects/assets/tetrahedron.obj", t3);

    double half_l = 4.2;
    double distance = 16;


    //MATERIAL
    Vector3 ka(0.2, 0.2, 0.2);
    Vector3 ks(1, 1, 1); //specular coef
    double ns = 32; //shininess;
    shared_mat mat_box1 = std::make_shared<Material>(ka, cols["yellow"], cols["black"], 32);
    auto mid_box1 = std::make_shared<TriangleMesh>(Vector3(-2, 5, -6.5),
                                                   std::make_shared<DiffuseBSDF>(mat_box1),
                                                   t1,
                                                   Vector3(2, 2, 2),
                                                   Vector3(0, 0, 45));

    shared_mat mat_box2 = std::make_shared<Material>(ka, cols["blue"], cols["black"], 32);
    auto mid_box2 = std::make_shared<TriangleMesh>(Vector3(0, 9, -6.5),
                                                   std::make_shared<DiffuseBSDF>(mat_box2),
                                                   t2,
                                                   Vector3(2, 2, 2),
                                                   Vector3(0, 0, 0));

    shared_mat mat_box3 = std::make_shared<Material>(ka, cols["red"], cols["black"], 32);
    auto mid_box3 = std::make_shared<TriangleMesh>(Vector3(-5, 7, -8.5),
                                                   std::make_shared<DiffuseBSDF>(mat_box3),
                                                   t3,
                                                   Vector3(4, 4, 4),
                                                   Vector3(0, 0, 15));

    shared_mat mat_light1 = std::make_shared<Material>(ka, cols["white"] * 0.2, ks, ns);
    mat_light1->set_emission(cols["white"] * 1.5, 0.4, 0, 1);
    shared_obj area_light1 = std::make_shared<Square>(Vector3(-half_l + 0.1, distance - 4, 0),
                                                      std::make_shared<DiffuseBSDF>(mat_light1),
                                                      Vector3(1, 0, 0),
                                                      Vector3(0, 0, 1),
                                                      1.5);

    shared_mat mat_light2 = std::make_shared<Material>(ka, cols["white"] * 0.2, ks, ns);
    mat_light2->set_emission(cols["white"] * 1.5, 0.4, 0, 1);
    shared_obj area_light2 = std::make_shared<Square>(Vector3(half_l - 0.1, distance - 4, 0),
                                                      std::make_shared<DiffuseBSDF>(mat_light2),
                                                      Vector3(-1, 0, 0),
                                                      Vector3(0, 0, 1),
                                                      1.5);

    //WALL
    shared_mat mat_f = std::make_shared<Material>(ka, cols["white"], cols["white"], 100);
    shared_obj floor = std::make_shared<Square>(Vector3(0, distance - half_l,
                                                        -half_l),
                                                std::make_shared<PhongBSDF>(mat_f),
                                                Vector3(0, 0, 1),
                                                Vector3(0, 1, 0),
                                                half_l);


    shared_bsdf wall_bsdf = std::make_shared<DiffuseBSDF>(std::make_shared<Material>(ka, cols["white"], ks, ns));
    shared_bsdf root_bsdf = std::make_shared<DiffuseBSDF>(std::make_shared<Material>(ka, cols["white"], ks, ns));
    shared_bsdf left_wall_bsdf = std::make_shared<DiffuseBSDF>(std::make_shared<Material>(ka, cols["red"], ks, ns));
    shared_bsdf right_wall_bsdf = std::make_shared<DiffuseBSDF>(std::make_shared<Material>(ka, cols["green"], ks, ns));

    shared_obj roof = std::make_shared<Square>(Vector3(0, distance - half_l, half_l),
                                               std::make_shared<PhongBSDF>(mat_f),
                                               Vector3(0, 0, -1),
                                               Vector3(0, 1, 0),
                                               half_l);

    shared_obj front_w = std::make_shared<Square>(Vector3(0, distance, 0),
                                                  wall_bsdf, Vector3(0, -1, 0),
                                                  Vector3(0, 0, 1),
                                                  half_l);


    shared_obj left_w = std::make_shared<Square>(Vector3(-half_l, distance - half_l, 0),
                                                 left_wall_bsdf, Vector3(1, 0, 0),
                                                 Vector3(0, 0, 1),
                                                 half_l);

    shared_obj right_w = std::make_shared<Square>(Vector3(half_l, distance - half_l, 0),
                                                  right_wall_bsdf, Vector3(-1, 0, 0),
                                                  Vector3(0, 0, 1),
                                                  half_l);
    Camera cam(Vector3(0, 0, 0), Vector3(0, 1, 0), Vector3(0, 0, 1), 60);
    Scene scene(400, 400, cam, Vector3(0.2, 0.2, 0.2));

    scene.add_object(mid_box1);
    scene.add_object(mid_box2);
    scene.add_object(mid_box3);
    scene.add_object(area_light1);
    scene.add_object(area_light2);
    scene.add_object(front_w);
    scene.add_object(roof);
    scene.add_object(floor);
    scene.add_object(left_w);
    scene.add_object(right_w);
    return scene;
}

void run_scene(Scene scene, int argc, char **argv) {
    double x = 0;
    double y = 0;
    srand48(std::time(0));
    Pathtracer pt(scene);
    parse_arguments(pt, argc, argv, x, y);
    pt.render();
    pt.to_ppm(pt.filename);
}

int main(int argc, char **argv) {
    if (argc > 0) {
        if (strcmp(argv[1], "cornell_box") == 0)
            run_scene(cornell_box(), argc, argv);
        else if (strcmp(argv[1], "multi_light") == 0)
            run_scene(multi_light(), argc, argv);
        else if (strcmp(argv[1], "pyramids") == 0)
            run_scene(pyramids(), argc, argv);
        else
            run_scene(simple(), argc, argv);
    } else {
        std::cout << "[USAGE]: main <available scene> --debug [--classic|--bdpt] [-fn filename] [-ns N_sample]"
                  << "[--indirect N_indirect_bounces] [--direct N_direct_bounces]\n"
                  << "available_scene: simple, cornell_box, pyramids, multi_light"
                  <<std::endl;
    }
    return 0;
}
