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

    Ray Object::sample_light_ray(float& pdf) const {
        //sample sphere surface
        double a = 2 * M_PI * rand1();
        double b = 2 * M_PI * rand1();
        Vector3 s(cos(a) * cos(b), cos(a) * sin(b), sin(a));
        pdf = 1.0 / (2.0 * M_PI);
        return Ray(position, s);
    }

    double Object::get_attenuation(double distance) const {
        return 1.0 / (material->a * distance * distance +
                      material->b * distance +
                      material->c);
    }

    Vector3 Object::get_color_from_light_point(shared_objs objects,
                                               const Vertex& light_point,
                                               const HitRecord& data,
                                               const Vector3& cam_dir) {
        Vector3 dir = light_point.pos - data.point; //from this obj to light
        float d = dir.norm();
        dir.normalize();
        float ld = data.normal.dot(dir);
        if (debug && debug_ray)
            std::cout << "light col " << light_point.col << "\n";

        Vector3 color;
        if (ld > 0)
        {
            //DIFFUSE LIGHT
            if (material->kd.max() > 0)
            {
                color += texture->get_color(data.point) * ld * material->kd *
                    objects[light_point.obj_id]->get_attenuation(d);
            }
            if (debug && debug_ray)
                std::cout << "light after diffuse: " << color << "\n";

            //SPECULAR LIGHT
            Vector3 r = dir.reflect(data.normal); //reflected vector
            float ls = cam_dir.dot(r) / (cam_dir.norm() * r.norm());

            if (ls > 0)
                color += material->ks * pow(ls, material->ns);

            if (debug && debug_ray)
                std::cout << "light after specular: " << color << "\n";
        }

        if (debug && debug_ray)
            std::cout << "color from vertex light:" << color * light_point.col
                      <<"\n";
        return color * light_point.col;
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
            if (mat->kd.max() > 0)
            {
                Vector3 diffuse_c = text->get_color(hit_data.point);
                float attenuation = 1.0 / (material->a * d * d + material->b * d
                                                         + material->c);
                color += diffuse_c * ld * mat->kd * attenuation;
            }
            if (debug && debug_ray)
                std::cout << "light after diffuse: " << color << "\n";

            //SPECULAR LIGHT
            Vector3 r = dir.reflect(hit_data.normal); //reflected vector
            float ls = cam_to_hit.dot(r) / (cam_to_hit.norm() * r.norm());

            if (ls > 0)
                color += mat->ks * pow(ls, mat->ns);

            if (debug && debug_ray)
                std::cout << "light after specular: " << color << "\n";
        }

        if (debug && debug_ray)
            std::cout << "light * emitted_rad:" << color
                      << this->emitted_rad <<"\n";
        return color * this->emitted_rad;
    }
}
