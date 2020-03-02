#include "light.hh"
#include <cmath>

namespace raytracing {
    PointLight::PointLight(const Vector3& color, const Vector3& position,
                           float constant, float linear, float quadratic)
        : DirectionalLight(color, position), constant(constant),
          linear(linear), quadratic(quadratic){}
    Vector3 PointLight::get_direction(const Vector3& point) const {
        return point - this->vector;
    }

    Vector3 PointLight::get_color(shared_texture texture,
                                  const HitRecord& hit_data) const
    {
        Vector3 dir = get_direction(hit_data.point);
        float distance = dir.norm();
        dir.normalize();


        float ld = hit_data.normal.dot(dir * -1);
        Vector3 color(0,0,0);
        if (ld > 0)
        {
            float kd = texture->get_kd(hit_data.point);
            Vector3 diffuse_color = texture->get_color(hit_data.point);
            float attenuation =  1.0 / (this->constant + this->linear * distance
                                        + this->quadratic * (distance * distance));

            color += diffuse_color * ld * kd * attenuation;
            color += get_specular_color(texture, hit_data, dir);
        }
        return color * this->color;
    }
}
