#include "parser/parser.hh"

using namespace pathtracing;

int main(int argc, char **argv) {
    //color
    Vector3 red(1,0,0);
    Vector3 green(0,1,0);
    Vector3 blue(0,0,1);
    Vector3 pink(1, 0, 0.75);
    Vector3 white(1,1,1);
    Vector3 black(0,0,0);
    //TEXTURE
    shared_text text1 = std::make_shared<UniformTexture>(Vector3(1,0.85,0.75));

    shared_text blue_white = std::make_shared<CheckedTexture>(Vector3(0,0,1),
                                                              Vector3(1,1,1));
    shared_text green_white = std::make_shared<CheckedTexture>(green, white);

    //MODEL
    shared_mod sp_small = std::make_shared<Sphere>(0.4);
    shared_mod sp_med = std::make_shared<Sphere>(1.0);
    shared_mod sp_l = std::make_shared<Sphere>(1.5);
    TriVector tv;
    shared_mod trimesh = std::make_shared<TriangleMesh>(tv, Vector3(1,1,1));

    double half_l = 4;
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
    double ka = 0.1; //ambient coef
    double kd = 0.5; //diffuse coef
    Vector3 ks(1, 1, 1); //specular coef
    double ns = 32; //shininess;
    double ni = 1;
    double kr = 0.5;
    double d = 1;

    //OBJECT
    //type, ka, kd, ks, ns, ni, d
    shared_mat mat2 = std::make_shared<Material>(Material::SPECULAR, ka, kd, ks, 0.01, ns, 1.33, 0.1);
    shared_obj o2 = std::make_shared<Object>(Vector3(1.5, 10, -2.4), mat2, blue,
                                             sp_l);

    //LIGHT OBJECT
    shared_mat mat3 = std::make_shared<Material>(Vector3(0.5, 0.5, 0.5), 0, 0.1, 1);

    shared_obj o3 = std::make_shared<Object>(Vector3(0,12,3.75), mat3, white, sp_small); //light source

    shared_mat mat4 = std::make_shared<Material>(Vector3(0.54, 0, 0.43), 0, 1, 1);

    shared_obj o4 = std::make_shared<Object>(Vector3(-2, 10,-3), mat4, pink, sp_small); //light source

    //WALL
    double distance = 16;
    shared_mat mat_f = std::make_shared<Material>(Material::SPECULAR, ka, kd, ks, 1, ns, ni, d);
    shared_obj floor = std::make_shared<Object>(Vector3(0, distance - half_l,
                                                        - half_l),
                                                mat_f, green_white, tb_wallm);

    shared_mat mat_w = std::make_shared<Material>(Material::DIFFUSE, ka, kd, ks, kr, ns, ni, d);

    shared_obj roof = std::make_shared<Object>(Vector3(0, distance - half_l, half_l),
                                               mat_w, blue, tb_wallm);
    shared_obj front_w = std::make_shared<Object>(Vector3(0, distance,0),
                                                  mat_w, text1, f_wallm);
    shared_obj left_w = std::make_shared<Object>(Vector3(-half_l, distance - half_l, 0),
                                                 mat_w, text1, lr_wallm);
    shared_obj right_w = std::make_shared<Object>(Vector3(half_l, distance - half_l, 0),
                                                  mat_w, text1, lr_wallm);
    Camera cam(Vector3(0,0,0), Vector3(0,1,0), Vector3(0,0,1), 60);
    Scene scene(500, 450, cam, Vector3(0.1, 0.1, 0.1));
//    scene.add_object(o1);
    scene.add_object(o2);
    scene.add_object(o3);
    scene.add_object(o4);
    scene.add_object(front_w);
    scene.add_object(roof);
    scene.add_object(floor);
    scene.add_object(left_w);
    scene.add_object(right_w);

    //Debug ray declaration
    int x = 0;
    int y = 0;
    srand48(std::time(0));
    Pathtracer pt(scene);
    parse_arguments(pt, argc, argv, x, y);
    if (debug_ray)
    {
        std::cout << pt.trace(x, y) << "\n";
    }
    else {
        pt.render();
        pt.to_ppm(pt.filename);
    }
    return 0;
}