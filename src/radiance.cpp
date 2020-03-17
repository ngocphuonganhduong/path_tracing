#include <iomanip>
#include "pathtracer.hh"

namespace pathtracing {

    Vector3 Pathtracer::get_direct_light_radiance(Vertices light_path,
                                             const HitRecord& hit_data,
                                             const Vector3 dir) {
        Vector3 rad;
        HitRecord tem;
        Vector3 dirl;
        shared_obj o = scene.objects[hit_data.obj_id];
        for (auto v: light_path)
        {
            //hared_obj lo = scene.objects[v.obj_id];
            dirl = hit_data.point - v.pos; //From light vertex to obj
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

                rad += o->get_color_from_light_point(scene.objects, v,
                                                     tem, dir);
                // col.clamp(0, max_intensity);
            }
        }
        return rad;
    }

    Vector3 Pathtracer::get_radiance(float x, float y, int niter, Vertices light_path)
    {
        Ray ray = scene.init_ray(x, y);
        bool is_continue = true;
        HitRecord hit_data;
        Vector3 rad;
        Vector3 dir;
        double scale = 1.0;
        shared_obj obj;
        shared_mat mat;
        double rnd;
        while (is_continue)
        {
            if (!scene.find_intersection(ray, hit_data))
                break;

            if (debug && debug_ray)
                std::cout << ray << "GET_RAD: obj_id: "<< hit_data.obj_id << "\n";

            if (hit_data.normal.dot(ray.get_direction()) > 0)
                hit_data.normal *= -1;

            obj = scene.objects[hit_data.obj_id];
            mat = obj->material;

            rad += (scene.ambient_light * mat->ka + obj->emitted_rad) * scale;

            //BIDIRECTIONAL PATHTRACING
            rad += get_direct_light_radiance(light_path, hit_data,
                                             ray.get_direction()) * scale;
            if (niter >= max_idl_bounce)
            {
                //ROUSSIAN ROULETTE to terminate path
                double p = mat->kr * terminate_param;
                if (rand1() < p)
                    scale /= p;
                else
                    break;
            }

            if (mat->kr < 0)
                break;
            //ROUSSIAN ROULETTE to decide with types of reflectance
            //3 types of reflectance: DIFFUSE, SPECULAR, GLOSSY
            double pd = (mat->kd.x + mat->kd.y + mat->kd.z)/3;
            double ps = (mat->ks.x + mat->ks.y + mat->ks.z)/3;
            rnd = rand1() * (pd + ps);
            scale *= mat->kr;
            if (rnd < pd) //DIFFUSE REFLECTANCE
            {
                dir = sample_diffuse(hit_data.normal);
            }
            else //SPECULAR/GLOSSY REFLECTANCE
            {
                if (rand1() < mat->ksm) {
                    dir = ray.get_direction().reflect(hit_data.normal);
                }
                else {
                    dir = sample_specular(hit_data.normal, mat->ns);
                }
                //float ls = dir.dot(ray.get_direction()) / (dir.norm() * ray.get_direction().norm());
                //if (ls > 0)
                //{
                //}
            }
            ray = Ray(hit_data.point, dir);
            niter += 1;
        }
        //TODO: refraction/opacity
        return rad;
    }
}
