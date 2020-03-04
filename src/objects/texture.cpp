#include "texture.hh"
namespace pathtracing {

    UniformTexture::UniformTexture(const Vector3& color_)
        : color(color_) {}

    Vector3 UniformTexture::get_color(const Vector3&) const {
        return this->color;
    }

    CheckedTexture::CheckedTexture(const Vector3& color_odd,
                                   const Vector3& color_even)
        : odd(color_odd), even(color_even){}

    Vector3 CheckedTexture::get_color(const Vector3 &p) const {
        float sines = sin(10 * p.x) * sin(10 * p.y) * sin(10 * p.z);
        if (sines < 0) {
            return this->odd;
        }
        return this->even;
    }

}
