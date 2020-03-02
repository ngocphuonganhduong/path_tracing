#ifndef IMAGE_H
#define IMAGE_H
#include "scene.hh"
#include <memory>
#include <string>

namespace raytracing {
    class Image {
    public:
        Image(Scene& scene);
        void tracing ();
        bool to_ppm(const char* filename);
        ~Image();
        void render(int starty, int endy);
        Vector3 get_color(float x, float y);
    private:
        Scene scene_;
        Vector3 *pixels;
        float max_ratio;
    };
}
#endif /* IMAGE_H */
