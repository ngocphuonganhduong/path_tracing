#include "pathtracer.hh"

namespace pathtracing {

    void Pathtracer::trace_light(Vertices& vertices, const Vertex& prev,
                                 const Ray &ray, const int bounces,
                                 float scale)
    {
        if (debug && debug_ray)
            std::cout << "TRACE LIGHT: " << ray << "; bounces" << bounces << "\n";

        double p = 1.0;
        if (bounces >= max_dl_bounce)
        {
            //ROUSSIAN ROULETTE to terminate path
            p = prev.col.max() * terminate_param;
            if (rand1() >=  p)
                return;
        }
        HitRecord hd; //hit data
        if (!scene.find_intersection(ray, hd))
            return;

        shared_obj o = scene.objects[hd.obj_id];
        shared_mat mat = o->material;

        //emissive color at this point ???
        Vector3 col = o->get_color_from_light_point(scene.objects, prev, hd,
                                                    ray.get_direction());
        col *= scale;
        col += o->emitted_rad;

        //col.clamp(0, max_intensity);
        Vertex v(hd.point, hd.obj_id, col);
        vertices.push_back(v);

        //ROUSSIAN ROULETTE to decide with types of reflectance
        //3 types of reflectance: DIFFUSE, SPECULAR, GLOSSY
        double pd = (mat->kd.x + mat->kd.y + mat->kd.z)/3;
        double ps = (mat->ks.x + mat->ks.y + mat->ks.z)/3;
        double rnd = rand1() * (pd + ps);

        Vector3 dir;
        if (rnd < pd) //DIFFUSE REFLECTANCE
        {
            dir = sample_diffuse(hd.normal);
            trace_light(vertices, v, Ray(hd.point, dir), bounces + 1, mat->kr);
        }
        else if (rnd < pd + ps) //SPECULAR/GLOSSY REFLECTANCE
        {
            if (rand1() < mat->ksm) {
                dir = ray.get_direction().reflect(hd.normal);
            }
            else {
                dir = sample_specular(hd.normal, mat->ns);
            }
            trace_light(vertices, v, Ray(hd.point, dir), bounces + 1, mat->kr);
        }

    }

    Vertices Pathtracer::generate_light_path()
    {
        float pdf = 1.0;
        Vertices vertices;
        for (unsigned int i = 0; i < scene.objects.size(); ++i)
        {
            if (scene.objects[i]->emitted_rad.max() > 0)
            {
                Ray sample_l = scene.objects[i]->sample_light_ray(pdf);
                Vertex v(sample_l.get_origin(), i,
                         scene.objects[i]->emitted_rad);
                vertices.push_back(v);

                trace_light(vertices, v, sample_l, 1,
                            scene.objects[i]->material->kr);
            }
        }
        return vertices;
    }

    Vector3 Pathtracer::trace(double x, double y)
    {
        Vertices light_path;
        if (max_dl_bounce > 0)
            light_path = generate_light_path();
        if (debug && debug_ray)
            std::cout << "nb of light vertices: " << light_path.size() << "\n";
        return get_radiance(x, y, 0, light_path);
    }
}
