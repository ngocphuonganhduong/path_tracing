#include "texture.hh"

namespace raytracing {
    UniformTexture::UniformTexture(const Vector3& color, float ka, float kd,
                                   float ks, float shininess,
                                   float reflectivity, const Vector3& specular)
        : color(color), ka_(ka), kd_(kd), ks_(ks), shininess_(shininess),
          reflectivity_(reflectivity), specular_(specular){}

    float UniformTexture::get_ka(const Vector3&) const {
        return this->ka_;
    }

    float UniformTexture::get_kd(const Vector3&) const {
        return this->kd_;
    }

    float UniformTexture::get_ks(const Vector3&) const {
        return this->ks_;
    }

    float UniformTexture::get_shininess(const Vector3&) const {
        return this->shininess_;
    }

    Vector3 UniformTexture::get_specular(const Vector3&) const {
        return this->specular_;
    }

    float UniformTexture::get_reflectivity(const Vector3&) const {
        return this->reflectivity_;
    }

    Vector3 UniformTexture::get_color(const Vector3&) const {
        return this->color;
    }


    CheckedTexture::CheckedTexture(const Vector3& color, const Vector3& color2,
                                   float ka, float kd, float ks,
                                   float shininess, float reflectivity,
                                   const Vector3& specular)
        : UniformTexture(color, ka, kd, ks, shininess, reflectivity, specular),
          color2(color2){}

    Vector3 CheckedTexture::get_color(const Vector3 &p) const {
        float sines = sin(10 * p.x) * sin(10 * p.y) * sin(10 * p.z);
        if (sines < 0) {
            return this->color;
        }
        return this->color2;
    }


}
