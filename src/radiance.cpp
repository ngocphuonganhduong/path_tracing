#include <iomanip>
#include "pathtracer.hh"

namespace pathtracing {

    Vector3 Pathtracer::get_direct_light_radiance(Vertices light_path,
                                                  const HitRecord &hit_data,
                                                  const Vector3 dir) {
        Vector3 rad;
        HitRecord tem;
        Vector3 dirl;
        shared_obj o = scene.objects[hit_data.obj_id];
        for (auto v: light_path) {
            //hared_obj lo = scene.objects[v.obj_id];
            dirl = hit_data.point - v.pos; //From light vertex to obj
            dirl.normalize();

            //shoot shadow ray from light to camera
            Ray shadow_ray(v.pos + dirl * 0.0001, dirl);

            if (debug && debug_ray)
                std::cout << "Shoot shadow ray: " << shadow_ray << "\n";

            //Check if it is visible (not hit any thing before hit the light)
            if (scene.find_intersection(shadow_ray, tem) &&
                tem.obj_id == hit_data.obj_id) {
                if (tem.normal.dot(dir) > 0)
                    tem.normal *= -1;

                rad += o->get_color_from_light_point(scene.objects, v,
                                                     tem, dir);
                // col.clamp(0, max_intensity);
            }
        }
        return rad;
    }

    Vector3 Pathtracer::get_radiance(const Ray &ray, int niter, Vertices light_path) {
        HitRecord hd;
        Vector3 rad;
        if (!scene.find_intersection(ray, hd))
            return rad;

        //if (debug && debug_ray)
        //    std::cout << ray << "GET_RAD: obj_id: "<< hd.obj_id << "\n";

        Vector3 dir = ray.get_direction();
        if (hd.normal.dot(dir) > 0)
            hd.normal *= -1;

        shared_mat mat = scene.objects[hd.obj_id]->material;

        rad += scene.ambient_light * mat->ka * mat->d + mat->ke;

        //BIDIRECTIONAL PATHTRACING
        rad += get_direct_light_radiance(light_path, hd, dir) * mat->d;

        double p = 1.0;
        if (niter >= max_idl_bounce) {
            //ROUSSIAN ROULETTE to terminate path
            double p = mat->ke.max() * terminate_param;
            if (drand48() >= p)
                return rad;
        }

        switch (mat->type) {
            case Material::DIFFUSE:
                dir = sample_diffuse(hd.normal);
                break;
            case Material::SPECULAR:
                dir = ray.get_direction().reflect(hd.normal);
                break;
            default:
                dir = sample_specular(hd.normal, mat->ns);
        }
        if (mat->d > 0)
            rad += get_radiance(Ray(hd.point, dir), niter + 1, light_path) * mat->kr * mat->d;
        if (mat->d < 1) {
            auto tmp = dir.refract(hd.normal, 1.0 / mat->ni);
            if (tmp != std::nullopt) {
                Ray r_ray(hd.point, tmp.value());
                Vector3 col = get_radiance(r_ray, niter + 1, light_path) * (1 - mat->kr) * (1 - mat->d);
                if (debug && debug_ray)
                    std::cout << "refraction: " << r_ray << "color: " << col << "\n";
                rad += col;
            }
        }
        return rad;
    }
}
