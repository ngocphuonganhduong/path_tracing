#include <stdio.h>
#include "raytracer.hh"

using namespace raytracing;

int main(int argc, char **argv)
{
    if (argc < 2)
    {
        std::cout << "Usage: rc [pixel x] [pixel y] \n";
        return 0;
    }
    //INIT SCENE AND CAST THE RAY
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
    return 0;
}
