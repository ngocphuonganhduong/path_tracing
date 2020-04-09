#include "render.hh"
#include <fstream>
#include <algorithm>

#define NB_THREADS 20


namespace pathtracing {
    bool debug = false;
    bool debug_ray = false;


    Pathtracer::Pathtracer(Scene &scene_)
            : scene(scene_) {
        s_size = 2; // n_sam = s_size x s_size
        n_sam = 4; // only subdivide pixel into square regions;
        area_size = 1.0 / 4.0;

        pixels = new uint8_t[scene_.width * scene_.height * 3];
        max_dl_bounce = 1;
        max_idl_bounce = 10;
        terminate_param = 0.8;
        max_intensity = 0.9;
        filename = "output.ppm";
        mode = Mode::PT;
    }

    Pathtracer::~Pathtracer() {
        delete[] pixels;
    }

    void Pathtracer::set_nb_samples(unsigned int ns) {
        s_size = sqrt(ns); // n_sam = s_size x s_size
        n_sam = s_size * s_size; // only subdivide pixel into square regions;
        area_size = 1.0 / double(s_size);
    }

    void Pathtracer::set_values(int x, int y, const Vector3 &color) {
        int i = (x + y * scene.width) * 3;
        pixels[i] = round(std::clamp(color.x(), 0.0, 1.0) * 255.0);
        pixels[i + 1] = round(std::clamp(color.y(), 0.0, 1.0) * 255.0);
        pixels[i + 2] = round(std::clamp(color.z(), 0.0, 1.0) * 255.0);
    }


    void Pathtracer::render_y(int thread_id, int starty, int endy, Vector3 (Pathtracer::*trace_func)(Ray)) {
        for (int y = starty; y < endy; ++y)
            for (int x = 0; x < scene.width; ++x) {
                Vector3 c;
                for (unsigned int sx = 0; sx < s_size; ++sx)
                    for (unsigned int sy = 0; sy < s_size; ++sy) {
                        c += (this->*trace_func)(scene.init_ray(double(x) - area_size / 2 + drand48() * area_size,
                                                                double(y) - area_size / 2 + drand48() * area_size));
                    }
                c /= n_sam;
                set_values(x, y, c);
            }
        if (debug)
            std::cout << "Thread " << thread_id << " is done\n";
    }

    void Pathtracer::render() {
        std::thread threads[NB_THREADS];
        int per_threads = scene.height / NB_THREADS;
        if (debug)
            std::cout << "Start Render scene(" << scene.width
                      << ", " << scene.height << ")\n";
        Vector3(Pathtracer::*trace_func)(Ray);
        switch (mode) {
            case CLASSIC_PT:
                if (debug)
                    std::cout << "Traditional PT (only implicit light, no explicit)\n";
                trace_func = &Pathtracer::trace_classic_pt;
                break;
            default:
                if (debug)
                    std::cout << "PT with explicit light\n";
                trace_func = &Pathtracer::trace_pt;
                break;
        }
        for (int i = 0; i < NB_THREADS; ++i) {
            threads[i] = std::thread(&Pathtracer::render_y, this, i,
                                     i * per_threads,
                                     (i + 1) * per_threads, trace_func);
        }
        render_y(-1, NB_THREADS * per_threads, scene.height, trace_func);
        for (int i = 0; i < NB_THREADS; ++i)
            threads[i].join();
    }

    bool Pathtracer::to_ppm(const std::string &filename) {
        std::ofstream ofs(filename, std::ofstream::out);
        if (!ofs.good()) {
            std::cerr << "ERROR: cannot open " << filename << " for writing"
                      << std::endl;
            return false;
        }

        ofs << "P3\n" << scene.width << " " << scene.height << std::endl;
        ofs << 255 << std::endl;
        for (int y = 0; y < scene.height; ++y)
            for (int x = 0; x < scene.width; ++x) {
                int i = (y * scene.width + x) * 3;
                ofs << unsigned(pixels[i]) << " "
                    << unsigned(pixels[i + 1]) << " "
                    << unsigned(pixels[i + 2]) << std::endl;
            }
        ofs.close();
        return true;
    }

}
