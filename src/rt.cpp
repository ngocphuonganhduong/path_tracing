#include <stdio.h>
#include "raytracer.hh"

using namespace raytracing;

int main() {
    float aspect_ratio = 4.0/3.0; // width/height
    Camera cam(Vector3(0,0,0), Vector3(0,1,0), Vector3(0,0,1), 60, aspect_ratio);

    Vector3 red(1,0,0);
    Vector3 blue(0,0,1);
    Vector3 green(0,1,0);
    Vector3 yellow(1,0.8,0);
    Vector3 light_blue(0.34, 0.74, 1);
    Vector3 specular(0.5, 0.5, 0.5);
    Vector3 pink(1.0,0,0.73);
    Vector3 purple(0.72, 0, 1);

    float s = 32; //shininess
    float ka = 1;
    float ks = 0.9;
    float kd = 0.8;
    shared_texture tred = std::make_shared<UniformTexture>(red, ka, kd, ks, s,
                                                           0.9, specular);
    shared_texture tblue = std::make_shared<UniformTexture>(blue, ka, kd, ks,
                                                            s, 0.3, specular);
    shared_texture tpink = std::make_shared<UniformTexture>(pink, ka, kd, ks,
                                                            s, 0.3, specular);
    shared_texture tyellow = std::make_shared<UniformTexture>(yellow, ka, kd, ks,
                                                            s, 0.9, specular);

    shared_texture tgreen = std::make_shared<UniformTexture>(green, ka, kd, ks,
                                                             s, 0.6, specular);
    shared_texture tpurple = std::make_shared<UniformTexture>(purple, ka, kd, ks,
                                                             s, 0.6, specular);

    shared_texture checked = std::make_shared<CheckedTexture>(green,
                                                              Vector3(1,1,1),
                                                              ka, kd, ks,
                                                              s, 0.4, specular);
    shared_texture twall = std::make_shared<UniformTexture>(light_blue, ka, kd, ks,
                                                            s, 0, specular);

    shared_texture roof = std::make_shared<UniformTexture>(red, ka, kd, ks,
                                                           s, 0.1, specular);

    float half_length = 3;
    float distance = 12;
    std::shared_ptr<Object> wall1 = std::make_shared<Square>(twall,
                                                             Vector3(0, distance, 0),
                                                             Vector3(0, -1, 0),
                                                             Vector3(0,0,1),
                                                             half_length);
    std::shared_ptr<Object> wall2 = std::make_shared<Square>(twall, Vector3(-half_length, distance - half_length, 0),
                                                             Vector3(1,0,0),
                                                             Vector3(0,0,1),
                                                             half_length);
    std::shared_ptr<Object> wall3 = std::make_shared<Square>(twall, Vector3(half_length, distance - half_length, 0),
                                                             Vector3(-1,0,0),
                                                             Vector3(0,0,1),
                                                             half_length);
    std::shared_ptr<Object> wall4 = std::make_shared<Square>(roof, Vector3(0, distance - half_length, half_length),
                                                             Vector3(0, 0, -1),
                                                             Vector3(0, 1, 0),
                                                             half_length);
    std::shared_ptr<Object> wall5 = std::make_shared<Square>(checked, Vector3(0, distance - half_length, -half_length),
                                                             Vector3(0, 0, -1),
                                                             Vector3(0, 1, 0),
                                                             half_length);

    float large = 1.25;
    float medium = 0.75;
    float small = 0.4;
    float mini = 0.25;
    std::shared_ptr<Object> sp2 = std::make_shared<Sphere>(tblue, Vector3(-1.25, distance - half_length + 0.5,  -half_length + medium), medium);

    std::shared_ptr<Object> sp3 = std::make_shared<Sphere>(tyellow, Vector3(0.75, distance - half_length + 1, -half_length + large), large);

    std::shared_ptr<Object> sp4 = std::make_shared<Sphere>(tpink, Vector3(2, distance - half_length - 0.5, -half_length + medium), medium);
    std::shared_ptr<Object> sp5 = std::make_shared<Sphere>(tpurple, Vector3(-2, distance - half_length - 1.5, -half_length + mini), mini);
    std::shared_ptr<Object> sp1 = std::make_shared<Sphere>(tblue, Vector3(0.5, distance - half_length + 1 - large - mini, -half_length + mini), mini);

    std::shared_ptr<Object> sp6 = std::make_shared<Sphere>(tred, Vector3(-0.75, distance - half_length - 2, -half_length + mini), mini);

    std::shared_ptr<Object> sp7 = std::make_shared<Sphere>(tgreen, Vector3(1, distance - half_length - 2.5, -half_length + small), small);

    std::shared_ptr<Light> ambient_light = std::make_shared<AmbientLight>(Vector3(0.1, 0.1, 0.1));
    std::shared_ptr<Light> dir_light = std::make_shared<DirectionalLight>(Vector3(0.2, 0.2, 0.2), Vector3(0, 2, 1));
    std::shared_ptr<Light> p_light = std::make_shared<PointLight>(Vector3(0.1, 0.1, 0.1), Vector3(-2.25, distance - half_length - 3 , -half_length + 0.2), 0.0, 0.7, 0);
    float height = 600;

    Scene scene(height * aspect_ratio, height, cam, 4);
    scene.add_object(sp1);
    scene.add_object(sp2);
    scene.add_object(sp3);
    scene.add_object(sp4);
    scene.add_object(sp5);
    scene.add_object(sp6);
    scene.add_object(sp7);
    scene.add_object(wall1);
    scene.add_object(wall2);
    scene.add_object(wall3);
    scene.add_object(wall4);
    scene.add_object(wall5);
    scene.add_light(ambient_light);
    scene.add_light(dir_light);
    scene.add_light(p_light);
    Raytracer rt(scene);
    rt.tracing();
    rt.to_ppm("output.ppm");
/*
    Vector3 color;//; = scene.get_color(ray, 0);
    //  std::cout << ray << " color" << color << "\n\n";

    Ray ray = scene.init_ray(434,286);
    color = scene.get_color(ray, 0);
    std::cout << ray << " color" << color << "\n\n";

    ray = scene.init_ray(442, 209);
    color = scene.get_color(ray, 0);
    std::cout << ray << " color" << color << "\n\n";
*/
    return 1;
}
