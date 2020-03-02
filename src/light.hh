#ifndef LIGHT_HH
#define LIGHT_HH

#include "ray.hh"
#include "object.hh"
#include <vector>

namespace raytracing {
    class Light
    {
    public:
        Light(const Vector3& color);
        virtual Vector3 get_color(shared_texture texture,
                                  const HitRecord& hit_data) const = 0;
        virtual float get_intensity(const Vector3& hit_point,
                                    const std::vector<shared_obj> objs) const = 0;

    protected:
        Vector3 color;
    };

    class AmbientLight : public Light {
    public:
        AmbientLight(const Vector3& color);
        Vector3 get_color(shared_texture texture, const HitRecord&) const final;
        float get_intensity(const Vector3& hit_point,
                            const std::vector<shared_obj> objs) const final;
    };


    class DirectionalLight : public Light {
    public:
        DirectionalLight(const Vector3& color, const Vector3& direction);
        virtual Vector3 get_color(shared_texture texture,
                          const HitRecord& hit_data) const;
        Vector3 get_specular_color(shared_texture texture,
                                   const HitRecord& hit_data,
                                   const Vector3& direction) const;
        float get_intensity(const Vector3& hit_point,
                            const std::vector<shared_obj> objs) const;
        virtual Vector3 get_direction(const Vector3& point) const;
    protected:
        Vector3 vector; //pos for point; direction for directional light

    };
    class PointLight : public DirectionalLight {
    public:
        PointLight(const Vector3& color, const Vector3& position,
                   float constant, float linear, float quadratic);
        Vector3 get_color(shared_texture texture,
                          const HitRecord& hit_data) const;
        Vector3 get_direction(const Vector3& point) const final;
        Vector3 get_diffuse_specular(shared_texture texture,
                                     const HitRecord& hit_data,
                                     const Vector3& sample) const;

    protected:
        float constant;
        float linear;
        float quadratic;
    };

    class SquareAreaLight : public PointLight {
    public:
        SquareAreaLight(const Vector3& color, const Vector3& center,
                        float constant, float linear, float quadratic,
                        const Vector3& normal, const Vector3& up,
                        float size, unsigned sample_size);
        Vector3 get_color(shared_texture texture,
                          const HitRecord& hit_data) const final;
        Vector3 get_sample(float x, float y) const;
        float get_intensity(const Vector3& hit_point,
                            const std::vector<shared_obj> objs) const final;
    private:
        Vector3 normal;
        Vector3 up;
        Vector3 right;
        float size;
        unsigned sample_size; //number of sample = sample size x sample size
        Vector3 top_left;
        float d; //size/sample_size
    };
}

#endif /* LIGHT_HH */
