#ifndef TEXTURE_HH
#define TEXTURE_HH
#include <memory>
#include "vector3.hh"

namespace raytracing {
    class Texture {
    public:
        virtual float get_ka(const Vector3& point) const = 0;
        virtual float get_kd(const Vector3& point) const = 0;
        virtual float  get_ks(const Vector3& point) const = 0;
        virtual float get_shininess(const Vector3& point) const = 0;
        virtual float get_reflectivity(const Vector3& point) const = 0;
        virtual Vector3 get_color(const Vector3 &point) const = 0;
        virtual Vector3 get_specular(const Vector3&) const = 0;
    };
    using shared_texture = std::shared_ptr<Texture>;

    class UniformTexture : public Texture
    {
    public:
        UniformTexture(const Vector3& color, float ka, float kd, float ks,
                       float shininess, float reflectivity,
                       const Vector3& specular);
        float get_ka(const Vector3& point) const final;
        float get_kd(const Vector3& point) const final;
        float get_ks(const Vector3& point) const final;
        float get_shininess(const Vector3& point) const final;
        float get_reflectivity(const Vector3& point) const final;
        Vector3 get_specular(const Vector3&) const final;
        Vector3 get_color(const Vector3 &point) const;
    protected:
        Vector3 color;
        float ka_;
        float kd_;
        float ks_;
        float shininess_;
        float reflectivity_;
        Vector3 specular_;
    };

    class CheckedTexture : public UniformTexture {
    public:
        CheckedTexture(const Vector3& color, const Vector3& color2, float ka,
                       float kd, float ks, float shininess, float reflectivity,
                       const Vector3& specular);

        Vector3 get_color(const Vector3 &point) const final;
    private:
        Vector3 color2;
    };
}
#endif /* TEXTURE_HH */
