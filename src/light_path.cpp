#include "pathtracer.hh"

namespace pathtracing {

    void Pathtracer::trace_light(Vertices& vertices, const Vertex& prev,
                                 const Ray &ray, const int bounces,
                                 float scale)
    {
        if (debug && debug_ray)
            std::cout << "TRACE LIGHT: " << ray << "; nth bounce: " << bounces << "\n";

        double p = 1.0;
        if (bounces >= max_dl_bounce)
        {
            //ROUSSIAN ROULETTE to terminate path
            p = prev.col.max() * terminate_param;
            if (drand48() >=  p)
                return;
        }
        HitRecord hd; //hit data
        if (!scene.find_intersection(ray, hd))
            return;


        if (hd.normal.dot(ray.get_direction()) > 0)
            hd.normal *= -1;
        shared_mat mat = scene.objects[hd.obj_id]->material;

        //emissive color at this point ???
        Vector3 col = scene.objects[hd.obj_id]->get_color_from_light_point(scene.objects, prev, hd,
                ray.get_direction());
        col *= scale;
        col += mat->ke;

        //col.clamp(0, max_intensity);
        Vertex v(hd.point, hd.obj_id, col);
        if (debug && debug_ray)
            std::cout << "obj id: " << hd.obj_id <<"\n";
        vertices.push_back(v);

        Vector3 dir;
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
        trace_light(vertices, v, Ray(hd.point, dir), bounces + 1, mat->kr);
    }

    Vertices Pathtracer::generate_light_path()
    {
        float pdf = 1.0;
        Vertices vertices;

        for (unsigned int i = 0; i < scene.objects.size(); ++i)
        {
            if (scene.objects[i]->material->ke.max() > 0)
            {
                Ray sample_l = scene.objects[i]->sample_light_ray(pdf);
                Vertex v(sample_l.get_origin(), i,
                         scene.objects[i]->material->ke);
                vertices.push_back(v);

                trace_light(vertices, v, sample_l, 1, 1);
            }
        }
        return vertices;
    }

    Vector3 Pathtracer::trace(double x, double y)
    {
        Vertices light_path;
        if (max_dl_bounce > 0)
            light_path = generate_light_path();
        if (debug && debug_ray) {
            std::cout << "nb of light vertices: " << light_path.size() << "\n";
            for (auto v: light_path)
                std::cout << "col: " << v.col << "\n";
        }
        Ray ray = scene.init_ray(x, y);
        return get_radiance(ray, 0, light_path);
    }
}
