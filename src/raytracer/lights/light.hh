#ifndef LIGHT_HH
#define LIGHT_HH
#include "../objects/object.hh"

namespace raytracing {
    class Light
    {
    public:
        Light(const Vector3& color): color(color) {}
        virtual Vector3 get_color(shared_texture texture,
                                  const HitRecord& hit_data) const = 0;

        //handling shadow/light intensity
        virtual float get_intensity(const Vector3& hit_point,
                                    const std::vector<shared_obj> objs) const = 0;
    protected:
        Vector3 color;
    };
    using shared_light = std::shared_ptr<Light>;

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
        float get_intensity(const Vector3& hit_point,
                            const std::vector<shared_obj> objs) const;
    protected:
        Vector3 get_specular_color(shared_texture texture,
                                   const HitRecord& hit_data,
                                   const Vector3& direction) const;
        virtual Vector3 get_direction(const Vector3& point) const;
        Vector3 vector; //pos for point; direction for directional light

    };

    class PointLight : public DirectionalLight {
    public:
        PointLight(const Vector3& color, const Vector3& position,
                   float constant, float linear, float quadratic);
        Vector3 get_color(shared_texture texture,
                          const HitRecord& hit_data) const;
    private:
        Vector3 get_direction(const Vector3& point) const final;
        float constant;
        float linear;
        float quadratic;
    };
}

#endif /* LIGHT_HH */
