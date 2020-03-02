#ifndef RAYTRACER_HH
#define RAYTRACER_HH
#include "scene.hh"

namespace raytracing {
    extern bool anti_aliasing_mode;
    extern bool shadow_mode;

    class Raytracer {
    public:
        Raytracer(Scene& scene);
        void tracing ();
        bool to_ppm(const char* filename);
        ~Raytracer();
        void render_anti_aliasing(int starty, int endy);
        void render_no_anti_aliasing(int starty, int endy);

        Vector3 get_color(float x, float y);

    private:
        Scene scene;
        Vector3 *pixels;
    };
}
#endif /* RAYTRACER_HH */
