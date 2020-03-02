#include "light.hh"
#include <cmath>

namespace raytracing {
    DirectionalLight::DirectionalLight(const Vector3& color,
                                       const Vector3& direction)
        : Light(color), vector(direction){}

    Vector3 DirectionalLight::get_direction(const Vector3&) const {
        return this->vector;
    }

    float DirectionalLight::get_intensity(const Vector3& hit_point,
                                          const std::vector<shared_obj> objs) const {
        Ray ray(hit_point, this->get_direction(hit_point) * -1);
        HitRecord tem;
        for (auto obj: objs)
        {
            if (obj->hit(ray, tem) && (tem.point - hit_point).norm() > 0.001)
            {
                return 0.0;
            }
        }
        return 1.0;

    }

    Vector3 DirectionalLight::get_specular_color(shared_texture texture,
                                                 const HitRecord& hit_data,
                                                 const Vector3& direction) const
    {
        Vector3 v = hit_data.direction * -1; //hit point to camera
        Vector3 r = direction.reflect(hit_data.normal);
        float ls = v.dot(r) / (v.norm() * r.norm()); //angle between v and r
        if (ls > 0)
        {
            float s = texture->get_shininess(hit_data.point);
            float ks = texture->get_ks(hit_data.point);
            Vector3 specular = texture->get_specular(hit_data.point);
            return specular * pow(ls, s) * ks;
        }
        return Vector3(0,0,0);
    }

    Vector3 DirectionalLight::get_color(shared_texture texture,
                                        const HitRecord& hit_data) const
    {
        float ld = hit_data.normal.dot(this->vector * -1);
        Vector3 color(0,0,0);
        if (ld > 0)
        {
            float kd = texture->get_kd(hit_data.point);
            Vector3 diffuse_color = texture->get_color(hit_data.point);
            color += diffuse_color * ld * kd;
            color += this->get_specular_color(texture, hit_data, this->vector);
        }
        return color * this->color;
    }
}
