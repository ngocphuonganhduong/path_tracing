#include "light.hh"

namespace raytracing {
    AmbientLight::AmbientLight(const Vector3& color)
        : Light(color){}

    Vector3 AmbientLight::get_color(shared_texture texture,
                                    const HitRecord& hit_data) const {
        return this->color * texture->get_ka(hit_data.point);
    }
    float AmbientLight::get_intensity(const Vector3&,
                                      const std::vector<shared_obj>) const {
        return 1.0;
    }

}
