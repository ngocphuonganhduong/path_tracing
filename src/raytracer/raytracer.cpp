#include "raytracer.hh"
#include <iomanip>
#include <thread>

#include <fstream>
#include <cmath>
#define NB_THREADS 20

namespace raytracing {
    bool anti_aliasing_mode = false;
    bool shadow_mode = true;

    Raytracer::Raytracer(Scene& scene_) : scene(scene_) {
        this->pixels = new Vector3[scene_.width * scene_.height];
    }
    Raytracer::~Raytracer(){
        delete[] pixels;
    }

    Vector3 Raytracer::get_color(float x, float y) {
        Ray ray = this->scene.init_ray(x, y);
        return  this->scene.get_color(ray, 0);
    }
    void Raytracer::render_no_anti_aliasing(int starty, int endy)
    {
        for (int y = starty; y < endy; ++y)
            for (int x = 0; x < scene.width; ++x)
            {
                Vector3 c = get_color(x, y);
                pixels[y * scene.width + x].set_values(std::min(1.0, c.x),//r
                                                       std::min(1.0, c.y),//g
                                                       std::min(1.0, c.z));//b
            }
    }
    void Raytracer::render_anti_aliasing(int starty, int endy)
    {
        Vector3 left_colors[3];
        float d = 0.5;
        for (int y = starty; y < endy; ++y)
        {
            //naive grid 3x3 sampling (9x9 = 9 rays per pixel)
            left_colors[0] = this->get_color(- d, y - d);
            left_colors[1] = this->get_color(- d, y);
            left_colors[2] = this->get_color(- d, y + d);
            for (int x = 0; x < scene.width; ++x)
            {
                Vector3 c;
                c += left_colors[0];
                c += left_colors[1];
                c += left_colors[2];
                for (float i = -d; i <= d; i += d)
                {
                    c += this->get_color(x, y + i);
                }
                left_colors[0] = this->get_color(x + d, y - d);
                left_colors[1] = this->get_color(x + d, y);
                left_colors[2] = this->get_color(x + d, y + d);
                c += left_colors[0];
                c += left_colors[1];
                c += left_colors[2];
                c /= 9;
                pixels[y * scene.width + x].set_values(std::min(1.0, c.x),//r
                                                       std::min(1.0, c.y),//g
                                                       std::min(1.0, c.z));//b

            }
        }
    }
    void Raytracer::tracing() {

        std::thread threads[NB_THREADS];
        int per_threads = scene.height / NB_THREADS;
        if (anti_aliasing_mode)
        {
            for (int i = 0; i < NB_THREADS; ++i)
            {
                threads[i] = std::thread(&Raytracer::render_anti_aliasing, this, i * per_threads, (i + 1) * per_threads);
            }
            render_anti_aliasing(NB_THREADS * per_threads, scene.height);
        }
        else
        {
            for (int i = 0; i < NB_THREADS; ++i)
            {
                threads[i] = std::thread(&Raytracer::render_no_anti_aliasing, this, i * per_threads, (i + 1) * per_threads);
            }
            render_no_anti_aliasing(NB_THREADS * per_threads, scene.height);
        }

        for(int i = 0; i < NB_THREADS; ++i)
            threads[i].join();
    }


    bool Raytracer::to_ppm(const char* filename){
        std::ofstream ofs (filename, std::ofstream::out);
        if (!ofs.good()) {
            std::cerr << "ERROR: cannot open " << filename << " for writing"
                      << std::endl;
            return false;
        }

        ofs << "P3\n" << scene.width << " " << scene.height << std::endl;
        ofs << 255 << std::endl;
        for (int y = 0; y <  scene.height; ++y)
            for (int x = 0; x < scene.width; ++x)
            {
                Vector3& p = pixels[y * scene.width + x];
                ofs << round(p.x * 255) << " "
                    << round(p.y * 255) << " "
                    << round(p.z * 255) << std::endl;
            }
        ofs.close();
        return true;
    }
}
