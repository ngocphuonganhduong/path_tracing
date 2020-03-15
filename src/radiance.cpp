#include <iomanip>
#include "pathtracer.hh"

namespace pathtracing {

    Vector3 Pathtracer::get_direct_light_radiance(Vertices light_path,
                                             const HitRecord& hit_data,
                                             const Vector3 dir) {
        Vector3 rad;
        HitRecord tem;

        shared_obj o = scene.objects[hit_data.obj_id];
        for (auto v: light_path)
        {
            //hared_obj lo = scene.objects[v.obj_id];
            Vector3 dirl = hit_data.point - v.pos; //From light vertex to obj
            dirl.normalize();

            //shoot shadow ray from light to camera
            Ray shadow_ray(v.pos + dirl * 0.0001, dirl);

            if (debug && debug_ray)
                std::cout << "Shoot shadow ray: "<< shadow_ray << "\n";

            //Check if it is visible (not hit any thing before hit the light)
            if (scene.find_intersection(shadow_ray, tem) &&
                tem.obj_id == hit_data.obj_id)
            {
                if (tem.normal.dot(dir) > 0)
                    tem.normal *= -1;

                Vector3 col = o->get_color_from_light_point(scene.objects, v,
                                                            tem, dir);
                col.clamp(0, max_intensity);
                if (debug && debug_ray)
                    std::cout << "light rad "<< col << "\n";

                rad += col;
            }
        }
        return rad;
    }

    Vector3 Pathtracer::get_radiance(const Ray& ray, int niter, Vertices light_path)
    {
        HitRecord hit_data;
        if (!scene.find_intersection(ray, hit_data))
            return Vector3(0,0,0);

        if (debug && debug_ray)
            std::cout << ray << "GET_RAD: obj_id: "<< hit_data.obj_id << "\n";

        if (hit_data.normal.dot(ray.get_direction()) > 0)
            hit_data.normal *= -1;

        shared_obj obj = scene.objects[hit_data.obj_id];
        shared_mat mat = obj->material;
        Vector3 obj_c = obj->texture->get_color(hit_data.point);
        Vector3 rad = scene.ambient_light * mat->ka + obj->emitted_rad;

        //BIDIRECTIONAL PATHTRACING
        rad += get_direct_light_radiance(light_path, hit_data,
                                         ray.get_direction());
        double scale = 1.0;
        if (niter >= max_idl_bounce)
        {
            //ROUSSIAN ROULETTE to terminate path
            double p = mat->kr * terminate_param;
            if (rand1() < p)
                scale /= p;
            else
                return rad;
        }

        if (mat->kr > 0)
        {
            //ROUSSIAN ROULETTE to decide with types of reflectance
            //3 types of reflectance: DIFFUSE, SPECULAR, GLOSSY
            double pd = (mat->kd.x + mat->kd.y + mat->kd.z)/3;
            double ps = (mat->ks.x + mat->ks.y + mat->ks.z)/3;
            double rnd = rand1() * (pd + ps);
            Vector3 dir;
            scale *= mat->kr;
            if (rnd < pd) //DIFFUSE REFLECTANCE
            {
                dir = sample_diffuse(hit_data.normal);
                Ray r_ray(hit_data.point, dir);
                rad += get_radiance(r_ray, niter + 1, light_path) * scale;
            }
            else if (rnd < pd + ps) //SPECULAR/GLOSSY REFLECTANCE
            {
                if (rand1() < mat->ksm) {
                    dir = ray.get_direction().reflect(hit_data.normal);
                }
                else {
                    dir = sample_specular(hit_data.normal, mat->ns);
                }
                Ray reflected_ray(hit_data.point, dir);
                rad += get_radiance(reflected_ray, niter + 1, light_path) * scale; //* mat->ks;
                //float ls = dir.dot(ray.get_direction()) / (dir.norm() * ray.get_direction().norm());
                //if (ls > 0)
                //{
                    //}
            }
        }
        //TODO: refraction/opacity
        return rad;
    }
}
