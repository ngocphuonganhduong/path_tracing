#include "light.hh"
#include <cmath>

namespace raytracing {
    Light::Light(const Vector3& color) : color(color) {}


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


    // DIRECTIONAL LIGHT
    DirectionalLight::DirectionalLight(const Vector3& color,
                                       const Vector3& direction)
        : Light(color), vector(direction){}

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
    Vector3 DirectionalLight::get_direction(const Vector3&) const{
        return this->vector;
    }

    Vector3 PointLight::get_direction(const Vector3& point) const{
        return point - this->vector;
    }
    PointLight::PointLight(const Vector3& color, const Vector3& position,
                           float constant, float linear, float quadratic)
        : DirectionalLight(color, position), constant(constant),
          linear(linear), quadratic(quadratic){}

    Vector3 PointLight::get_diffuse_specular(shared_texture texture,
                                             const HitRecord& hit_data,
                                             const Vector3& sample) const {
        Vector3 dir = hit_data.point - sample;

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
            color += diffuse_color * ld * kd * attenuation;//sqrt(distance);
            color += this->get_specular_color(texture, hit_data, dir);
        }
        return color * this->color;
    }

    Vector3 PointLight::get_color(shared_texture texture,
                                  const HitRecord& hit_data) const
    {
        return this->get_diffuse_specular(texture, hit_data, this->vector);
    }
    SquareAreaLight::SquareAreaLight(const Vector3& color, const Vector3& center,
                                     float constant, float linear, float quadratic,
                                     const Vector3& normal, const Vector3& up,
                                     float size, unsigned sample_size)
        : PointLight(color, center, constant, linear, quadratic)
    {
        this->sample_size = sample_size;
        this->size = size;
        this->d = size / float(sample_size);

        this->normal = normal;
        this->up = up;
        this->right = normal.cross_product(up);
        this->up.normalize();
        this->right.normalize();

        this->top_left = center - this->right/2 + this->up/2;
    }
    Vector3 SquareAreaLight::get_color(shared_texture texture,
                                       const HitRecord& hit_data) const {
        Vector3 color;
        for (unsigned y = 0; y < sample_size; y += 1)
            for (unsigned x = 0; x < sample_size; x += 1)
            {
                Vector3 sample = this->get_sample(x, y);
                color += this->get_diffuse_specular(texture, hit_data, sample);
            }

        return color / (sample_size * sample_size);
}
    Vector3 SquareAreaLight::get_sample(float x, float y) const {
        return this->top_left
            + this->right * (x + rand1() * d ) * this->size / this->sample_size
            - this->up * (y + rand1() * d ) * this->size / this->sample_size;
    }
    float SquareAreaLight::get_intensity(const Vector3& hit_point,
                                         const std::vector<shared_obj> objs) const {
        float i = 0.0;
        HitRecord tem;
        for (unsigned y = 0; y < this->sample_size; y += 1)
            for (unsigned x = 0; x < this->sample_size; x += 1)
            {
                Vector3 sample = this->get_sample(x, y);
                Ray ray(hit_point, sample - hit_point);
                bool hit = false;
                for (int i = 0; i < objs.size() && !hit; ++i)
                {
                    if (objs[i]->hit(ray, tem) && (tem.point - hit_point).norm() > 0.001)
                    {
                        hit = true;
                    }
                }
                if (!hit) {
                    i += 1;
                }
            }
        return i / (this->sample_size * this->sample_size);
    }
}
