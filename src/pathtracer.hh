#ifndef PATHTRACER_HH
#define PATHTRACER_HH
#include "scene.hh"
#include <thread>

namespace pathtracing {
    extern bool debug;
    extern bool debug_ray;
    class Pathtracer {
    public:
        Pathtracer(Scene& scene);

        ~Pathtracer();
        void render ();
        bool to_ppm(const std::string& filename);
        Vector3 trace(double x, double y);
        void set_nb_samples(unsigned int ns);
        int max_dl_bounce; //max nb of direct light bounces
        int max_idl_bounce; //max nb of indirect light bounces
        std::string filename;

    private:
        void set_values(int x, int y, const Vector3& color);
        void render_y(int thread_id, int starty, int endy);
        Vertices generate_light_path();
        void trace_light(Vertices& vertices, const Vertex& prev,
                         const Ray &ray, const int bounces, float scale);

        /**
         * @param light_path a list of vertices tracing light
         * @param hit_data data containing info abt hit point
         * @param direction of cam/eye to the hit_point
         **/
        Vector3 get_direct_light_radiance(Vertices light_path,
                                          const HitRecord& hit_data,
                                          const Vector3 dir);
        Vector3 get_radiance(const Ray& ray, int niter, Vertices light_path);
        Scene scene;
        unsigned int n_sam; //nb of samples per ray
        unsigned int s_size; // n_sam = s_size x s_size
        double area_size; // edge size of the area containing a sample;
                          //area_size x area_size
        double terminate_param; // (0, 1.0)
        double max_intensity; // clamp the intensity of the light each bounce
        uint8_t *pixels;
    };
}



#endif /* PATHTRACER_HH */
