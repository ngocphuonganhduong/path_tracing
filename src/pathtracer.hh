#ifndef PATHTRACER_HH
#define PATHTRACER_HH
#include "scene.hh"
#include <thread>

namespace pathtracing {
    extern bool debug;
    extern bool debug_ray;
    class Pathtracer {
    public:
        Pathtracer(Scene& scene, unsigned int n_sam);
        ~Pathtracer();
        void render ();
        bool to_ppm(const char* filename);
        Vector3 trace(double x, double y);

    private:
        void set_values(int x, int y, const Vector3& color);
        void render_y(int thread_id, int starty, int endy);
        Scene scene;
        unsigned int n_sam; //nb of samples per ray
        unsigned int s_size; // n_sam = s_size x s_size
        double area_size; // edge size of the area containing a sample;
                          //area_size x area_size
        uint8_t *pixels;
    };
}



#endif /* PATHTRACER_HH */
