#include "image.hh"
#include <iomanip>
#include <thread>

#include <fstream>
#include <cmath>
#define NB_THREADS 20

namespace raytracing {
    Image::Image(Scene& scene) : scene_(scene), max_ratio(1.0) {
        this->pixels = new Vector3[scene.get_width() * scene.get_height() ];
    }
    Image::~Image(){
        delete[] pixels;
    }
    Vector3 Image::get_color(float x, float y) {
        Ray ray = this->scene_.init_ray(x, y);
        return  this->scene_.get_color(ray, 0);
    }
    void Image::render(int starty, int endy)
    {
        int w = this->scene_.get_width();
        Vector3 left_colors[3];
        float d = 0.5;
        float m = 1.0;
        for (int y = starty; y < endy; ++y)
        {
            left_colors[0] = this->get_color(- d, y - d);
            left_colors[1] = this->get_color(- d, y);
            left_colors[2] = this->get_color(- d, y + d);
            for (int x = 0; x < w; ++x)
            {
                Vector3 color;
                color += left_colors[0];
                color += left_colors[1];
                color += left_colors[2];
                /*
                Ray ray = this->scene_.init_ray(x, y);
                color += this->scene_.get_color(ray, 0);
                */
                for (float i = -d; i <= d; i += d)
                {
                    color += this->get_color(x, y + i);
                }
                left_colors[0] = this->get_color(x + d, y - d);
                left_colors[1] = this->get_color(x + d, y);
                left_colors[2] = this->get_color(x + d, y + d);
                color += left_colors[0];
                color += left_colors[1];
                color += left_colors[2];
                color /= 9;
                float r = std::min(m, color.r());
                float g = std::min(m, color.g());
                float b = std::min(m, color.b());
                this->pixels[y * w + x].set_values(r,g,b);

            }
        }
    }
    void Image::tracing() {
        int h = this->scene_.get_height();
        HitRecord hit_point;
        this->max_ratio = 1.0;
        std::thread threads[NB_THREADS];
        int per_threads = h/NB_THREADS;

        for (int i = 0; i < NB_THREADS; ++i)
        {
            threads[i] = std::thread(&Image::render, this, i * per_threads, (i + 1) * per_threads);
        }
        for(int i = 0; i < NB_THREADS; ++i)
            threads[i].join();
        this->render(NB_THREADS * per_threads, h);
    }


    bool Image::to_ppm(const char* filename){
        std::ofstream ofs (filename, std::ofstream::out);
        if (!ofs.good()) {
            std::cerr << "ERROR: cannot open " << filename << " for writing"
                      << std::endl;
            return false;
        }

        int h = this->scene_.get_height();
        int w = this->scene_.get_width();

        ofs << "P3\n" << w << " " << h << std::endl;
        ofs << 255 << std::endl;
        for (int y = 0; y < h; ++y)
            for (int x = 0; x < w; ++x)
            {
                Vector3& p = pixels[y * w + x];
                ofs << round(p.r() * 255) << " "
                    << round(p.g() * 255) << " "
                    << round(p.b() * 255) << std::endl;
            }
        ofs.close();
        return true;
    }
}
