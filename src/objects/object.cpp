#include "../render.hh"
#include "../utils/sampler.hh"

namespace pathtracing {


    Object::Object(const Vector3 &pos_, shared_mat mat_)
            : position(pos_), material(mat_) {}
//
//
//    Ray Object::sampleLightRay(double &pdfForward, double &pdfBackward) const {
//        Vector3 normal;
//        Vector3 pos = sampleSurfacePosition(pdfBackward);
//        return Ray(pos, sampleHemisphere(pdfForward, normal));
//    }


    Vector3 Object::phong_lighting(const Vector3 &li, const Vector3 &wi, const Vector3 &wo, const Vector3 &normal,
                                   const double &d2) {
        Vector3 dir = wi * -1;
        float ld = normal.dot(dir);
        Vector3 color;
        if (ld > 0) {
            //DIFFUSE LIGHT
            color += material->kd * ld / d2;

            Vector3 r = dir.reflect(normal); //reflected vector
            float ls = wo.dot(r) / (wo.norm() * r.norm());

            if (ls > 0)
                color += material->ks * pow(ls, material->ns);
        }
        return color;
    }

//    Vector3 Object::from_direct_light(shared_obj light, const Vector3 &pos, const HitRecord &data,
//                                      const Vector3 &wo) {
//        Vector3 dir = pos - data.point; //from this obj to light
//        float d = dir.norm();
//        dir.normalize();
//        float ld = data.normal.dot(dir);
//
//        Vector3 color;
//        if (ld > 0) {
//            //DIFFUSE LIGHT
//            color += texture->get_color(data.point) * ld * material->kd *
//                     light->get_attenuation(d);
//            if (debug && debug_ray)
//                std::cout << "light after diffuse: " << color << "\n";
//
//            //SPECULAR LIGHT
//            Vector3 r = dir.reflect(data.normal); //reflected vector
//            float ls = wo.dot(r) / (wo.norm() * r.norm());
//
//            if (ls > 0)
//                color += material->ks * pow(ls, material->ns);
//
//            if (debug && debug_ray)
//                std::cout << "light after specular: " << color << "\n";
//        }
//
//        return color * light->material->ke;
//    }

//    Vector3 Object::get_emitted_at(shared_mat mat, shared_text text,
//                                   const HitRecord &hit_data,
//                                   const Vector3 &cam_to_hit,
//                                   const Vector3 &hpoint_to_light) {
//        Vector3 dir = hpoint_to_light;
//        float d = dir.norm();
//        dir.normalize();
//
//        float ld = hit_data.normal.dot(dir);
//        Vector3 color;
//        if (ld > 0) {
//            //DIFFUSE LIGHT
//            if (mat->kd > 0) {
//                Vector3 diffuse_c = text->get_color(hit_data.point);
//                float attenuation = 1.0 / (material->a * d * d + material->b * d
//                                           + material->c);
//                color += diffuse_c * ld * mat->kd * attenuation;
//            }
//            if (debug && debug_ray)
//                std::cout << "light after diffuse: " << color << "\n";
//
//            //SPECULAR LIGHT
//            Vector3 r = dir.reflect(hit_data.normal); //reflected vector
//            float ls = cam_to_hit.dot(r) / (cam_to_hit.norm() * r.norm());
//
//            if (ls > 0)
//                color += mat->ks * pow(ls, mat->ns);
//
//            if (debug && debug_ray)
//                std::cout << "light after specular: " << color << "\n";
//        }
//
//        if (debug && debug_ray)
//            std::cout << "light * emitted_rad:" << color
//                      << this->material->ke << "\n";
//        return color;// * this->material->ke;
//    }
}
