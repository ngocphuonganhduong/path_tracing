#include "parser/parser.hh"

using namespace pathtracing;

int main(int argc, char **argv) {
    //color
    Vector3 red(1, 0, 0);
    Vector3 green(0, 1, 0);
    Vector3 blue(0, 0, 1);
    Vector3 pink(1, 0.6, 0.8);
    Vector3 white(1, 1, 1);
    Vector3 black(0, 0, 0);
    Vector3 yellow(1, 1, 0.2);
    Vector3 cyan(0, 1, 1);
    Vector3 purple(0.7, 0, 1);
    Vector3 wall_color (0.95, 0.92, 0.97);
    double half_l = 4;

    //MATERIAL
    Vector3 ka(0.1, 0.1, 0.1);
    Vector3 ks(1, 1, 1); //specular coef
    double ns = 32; //shininess;
    double ni = 1;
    double kr = 0.5;
    double d = 1;

    //OBJECT
    //type, ka, kd, ks, ns, ni, d
    shared_mat mat = Material::create_mat(Material::BLINN_PHONG, ka, pink*0.5, white*0.5, 100);
    auto o1 = std::make_shared<Sphere>(Vector3(1.5, 11, -2.5), mat, 1.4);

    shared_mat mat2 = Material::create_mat(Material::DIFFUSE, ka, yellow, white, 32);
    auto o2 = std::make_shared<Sphere>(Vector3(-2, 10, -3), mat2, 0.8);

    shared_mat mat_light = Material::create_mat(ka, white, ks, ns);

    double distance = 16;

    mat_light->set_emission(white *0.9, 0.05, 1, 1);
    shared_obj area_light = std::make_shared<Square>(Vector3(0, distance - 6, half_l-0.01),
                                                     mat_light, Vector3(0, 0, -1),
                                                     Vector3(0, 1, 0),
                                                     2);

    //WALL
    shared_mat mat_f = Material::create_mat(Material::DIFFUSE, ka, green , ks, 32);
    shared_obj floor = std::make_shared<Square>(Vector3(0, distance - half_l,
                                                        -half_l),
                                                mat_f,
                                                Vector3(0, 0, 1),
                                                Vector3(0, 1, 0),
                                                half_l);

    shared_mat mat_w = Material::create_mat(ka, white, ks, ns);

    shared_mat mat_roof = Material::create_mat(ka, wall_color , ks, ns);

    shared_obj roof = std::make_shared<Square>(Vector3(0, distance - half_l, half_l),
                                               mat_roof, Vector3(0, 0, -1),
                                               Vector3(0, 1, 0),
                                               half_l);

    shared_obj front_w = std::make_shared<Square>(Vector3(0, distance, 0),
                                                  mat_w, Vector3(0, -1, 0),
                                                  Vector3(0, 0, 1),
                                                  half_l);

    shared_mat mat_left = Material::create_mat(ka, red , ks, ns);

    shared_obj left_w = std::make_shared<Square>(Vector3(-half_l, distance - half_l, 0),
                                                 mat_left, Vector3(1, 0, 0),
                                                 Vector3(0, 0, 1),
                                                 half_l);
    shared_mat mat_right = Material::create_mat(ka, blue , ks, ns);
    shared_obj right_w = std::make_shared<Square>(Vector3(half_l, distance - half_l, 0),
                                                  mat_right, Vector3(-1, 0, 0),
                                                  Vector3(0, 0, 1),
                                                  half_l);
    Camera cam(Vector3(0, 0, 0), Vector3(0, 1, 0), Vector3(0, 0, 1), 60);
    Scene scene(500, 450, cam, Vector3(0.2, 0.2, 0.2));

    scene.add_object(o1);
    scene.add_object(o2);
//    scene.add_object(o3);
    scene.add_object(area_light);
    scene.add_object(front_w);
    scene.add_object(roof);
    scene.add_object(floor);
    scene.add_object(left_w);
    scene.add_object(right_w);

    //Debug ray declaration
    double x = 0;
    double y = 0;
    srand48(std::time(0));
    Pathtracer pt(scene);
    parse_arguments(pt, argc, argv, x, y);
    if (debug_ray) {
        if (pt.mode == CLASSIC_PT)
            std::cout << pt.trace_classic_pt(scene.init_ray(x, y)) << "\n";
        else
            std::cout << pt.trace_pt(scene.init_ray(x, y)) << "\n";
    } else {
        pt.render();
        pt.to_ppm(pt.filename);
    }
    return 0;
}
