# include "parser/parser.hh"
# include "objects/object.hh"
# include "objects/object_loader.hh"

using namespace pathtracing;

int main(int argc, char **argv) {
    
    objl::Loader loader;
    TriVector t;
    TriVector t2;

    loader.load_object("../src/objects/assets/box.obj", t);
    loader.load_object("../src/objects/assets/tall_box.obj", t2);
    
    //color
    Vector3 red(1, 0.4, 0.4);
    Vector3 green(0.4, 1, 0.4);
    Vector3 blue(0.4, 0.4, 1);
    Vector3 pink(1, 0.6, 0.8);
    Vector3 white(1, 1, 1);
    Vector3 black(0, 0, 0);
    Vector3 yellow(1, 1, 0.2);
    Vector3 cyan(0, 1, 1);
    Vector3 purple(0.7, 0, 1);
    Vector3 wall_color(0.95, 0.92, 0.97);
    double half_l = 4.2;
    double distance = 16;


    //MATERIAL
    Vector3 ka(0.2, 0.2, 0.2);
    Vector3 ks(1, 1, 1); //specular coef
    double ns = 32; //shininess;
    double ni = 1;
    double kr = 0.5;
    double d = 1;

    shared_mat mat = std::make_shared<Material>(ka, white * 0.5 , white, 32);
    // the small box
    auto o1 = std::make_shared<TriangleMesh>(Vector3(2, 11, -3), std::make_shared<DiffuseBSDF>(mat), t, Vector3(1.2,1.2,1.2), Vector3(0,0,-30));

    shared_mat mat2 = std::make_shared<Material>(ka, white, white, 32);
    // the tall box
    auto o2 = std::make_shared<TriangleMesh>(Vector3(-2, 12.5, -2), std::make_shared<DiffuseBSDF>(mat2), t2, Vector3(1.2,1.2,1.2), Vector3(0,0,30));

    // the sphere
    auto o3 = std::make_shared<Sphere>(Vector3(-2, 12.5, 1.4), std::make_shared<MirrorBSDF>(mat2), 1.0);

    shared_mat mat_light = std::make_shared<Material>(ka, white, ks, ns);
    mat_light->set_emission(white * 6, 0.2, 0, 1);


    shared_obj area_light = std::make_shared<Square>(Vector3(0, distance - 4, half_l - 0.1),
                                                     std::make_shared<DiffuseBSDF>(mat_light), Vector3(0, 0, -1),
                                                     Vector3(0, 1, 0),
                                                     1.5);

    //WALL
    shared_mat mat_f = std::make_shared<Material>(ka, green, white, 100);
    shared_obj floor = std::make_shared<Square>(Vector3(0, distance - half_l,
                                                        -half_l),
                                                std::make_shared<PhongBSDF>(mat_f),
                                                Vector3(0, 0, 1),
                                                Vector3(0, 1, 0),
                                                half_l);


    shared_bsdf wall_bsdf = std::make_shared<DiffuseBSDF>(std::make_shared<Material>(ka, white, ks, ns));
    shared_bsdf root_bsdf = std::make_shared<DiffuseBSDF>(std::make_shared<Material>(ka, white, ks, ns));
    shared_bsdf left_wall_bsdf = std::make_shared<DiffuseBSDF>(std::make_shared<Material>(ka, red, ks, ns));
    shared_bsdf right_wall_bsdf = std::make_shared<DiffuseBSDF>(std::make_shared<Material>(ka, blue, ks, ns));


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
    Scene scene(700, 700, cam, Vector3(0.2, 0.2, 0.2));

    scene.add_object(o1);
    scene.add_object(o2);
    scene.add_object(o3);
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
