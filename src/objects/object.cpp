#include "../pathtracer.hh"

namespace pathtracing {

    Object::Object(const Vector3& pos_, const Vector3& e_rad_, shared_mat mat_,
                   shared_text text_, shared_mod model_)
        : position(pos_), emitted_rad(e_rad_), material(mat_), texture(text_),
          model(model_) {}

    Object::Object(const Vector3& pos_, shared_mat mat_, shared_text text_,
                   shared_mod model_)
        : position(pos_), material(mat_), texture(text_), model(model_) {}

    Object::Object(const Vector3& pos_, const Vector3& e_rad_, shared_mat mat_,
           const Vector3& color, shared_mod model_)
        : position(pos_), emitted_rad(e_rad_), material(mat_),
          texture(std::make_shared<UniformTexture>(color)), model(model_) {}

    Object::Object(const Vector3& pos_, shared_mat mat_, const Vector3& color,
           shared_mod model_)
        : position(pos_), material(mat_),
          texture(std::make_shared<UniformTexture>(color)), model(model_) {}


    bool Object::hit(const Ray& r, HitRecord& hit_data) const
    {
        return model->hit(position, r, hit_data);
    }

    Vector3 Object::get_sample() const {
        return model->get_sample(position);
    }
    Vector3 Object::get_emitted_at(shared_mat mat, shared_text text,
                                   const HitRecord& hit_data,
                                   const Vector3& cam_to_hit,
                                   const Vector3& hpoint_to_light) {
        Vector3 dir = hpoint_to_light;
        float d = dir.norm();
        dir.normalize();

        float ld = hit_data.normal.dot(dir);
        Vector3 color;
        if (ld > 0)
        {
            //DIFFUSE LIGHT
            if (mat->kd > 0)
            {
                Vector3 diffuse_c = text->get_color(hit_data.point);
                float attenuation = 1.0 / (material->a * d * d + material->b * d
                                                         + material->c);
                color += diffuse_c * ld * mat->kd * attenuation;
            }
            //SPECULAR LIGHT
            Vector3 r = dir.reflect(hit_data.normal); //reflected vector
            float ls = cam_to_hit.dot(r) / (cam_to_hit.norm() * r.norm());
            if (ls > 0)
                color += mat->specular * pow(ls, mat->shininess) * mat->ks;
        }
        if (debug && debug_ray)
            std::cout << "light * emitted_rad:" << color
                      << this->emitted_rad <<"\n";
        return color * this->emitted_rad;
    }
}
