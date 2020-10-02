#ifndef PATHTRACER_HH
#define PATHTRACER_HH

#include "scene.hh"
#include "utils/matrix.hh"
#include <thread>
//#define DEBUG_RAY
#define NB_THREADS 20

#define EPSILON 0.000001

namespace pathtracing {
    extern bool debug;
    extern bool debug_ray;

    enum Mode {
        CLASSIC_PT = 0, PT, BDPT
    };

    class Pathtracer {
    public:
        Pathtracer(Scene &scene);

        ~Pathtracer();

        void render();

        bool to_ppm(const std::string &filename);

        Vector3 trace_pt(Ray ray);

        Vector3 trace_classic_pt(Ray ray);

        Vector3 trace_bdpt(Ray ray);

        void set_nb_samples(unsigned int ns);

        int max_dl_bounce; //max nb of direct light bounces
        int max_idl_bounce; //max nb of indirect light bounces
//        int max_path_length = 5; //max nb of indirect light bounces
        std::string filename;
        Mode mode;

    private:
        void set_values(int x, int y, const Vector3 &color);

        void render_y(int thread_id, int starty, int endy, Vector3 (Pathtracer::*trace_func)(Ray));

        Vector3 visibility_test(const Vector3& position, const Vector3& direction, unsigned int obj_id) const;
        std::vector<Vertex> generate_light_path();
        std::vector<Vertex> generate_eye_path(Ray ray);

//        void trace_light_ray(Path &vertices, Ray ray_in, Vector3 cumulative);

//        Vector3 computeDirectLighting(const Paths &lightPaths, const HitRecord &hd, const Vector3 &wo);


        /**
         * @param light_path a list of vertices tracing light
         * @param hit_data data containing info abt hit point
         * @param direction of cam/eye to the hit_point
         **/
//        Vector3 get_direct_light_radiance(Vertices light_path,
//                                          const HitRecord &hit_data,
//                                          const Vector3 dir);

//        Vector3 get_radiance(const Ray &ray, int niter, Paths light_path);

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
