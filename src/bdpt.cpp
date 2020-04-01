#include "render.hh"
#include "utils/matrix.hh"

namespace pathtracing {


    Vector3 Pathtracer::computeDirectLighting(const Paths &lightPaths, const HitRecord &hd, const Vector3 &wo) {
        Vector3 rad;
        HitRecord tem;
        Vector3 li;
        for (auto path: lightPaths) {
            Vector3 wi = hd.point - path[0].data.point; //From light vertex to obj
            double d2 = wi.norm_square();
            wi.normalize();
            if (scene.find_intersection(Ray(path[0].data.point + wi * EPSILON, wi), tem) && tem.obj_id == hd.obj_id) {
                //li * brdf * cos(normal, w out)
//                if (debug && debug_ray) {
//                    std::cout << "light" << path[0].li << "\n";
//                    std::cout << "brdf" << scene.objects[hd.obj_id]->material->brdf(wi, wo) << "\n";
//                    std::cout << "normal " << hd.normal << "\n";
//                    std::cout << "wi " << wi << "\n";
//                    std::cout << "cos theta" << Vector3::cos(hd.normal, wi) << "\n";
//                }
                rad += scene.objects[hd.obj_id]->phong_lighting(li, wi * -1, wo, hd.normal, d2);
//                rad += path[0].li * scene.objects[hd.obj_id]->material->bsdf->f(wi, wo) * Vector3::cos(hd.normal, wi) / d2;
            }

            for (unsigned i = 1; i < path.size(); ++i) {
                wi = (hd.point - path[i].data.point);
                d2 = wi.norm_square();
                wi.normalize();
                if (scene.find_intersection(Ray(path[i].data.point + wi * EPSILON, wi), tem) &&
                    tem.obj_id == hd.obj_id) {

                    d2 *= (path[i - 1].data.point - path[i].data.point).norm_square();

                    //re evaluate li
                    li = path[i - 1].li * scene.objects[path[i].data.obj_id]->material->bsdf->f(path[i].wi, wi) *
                         fabs(Vector3::cos(path[i].data.normal, wi));

                    //li * brdf * cos(normal, w out)
//                    rad += li * scene.objects[hd.obj_id]->material->bsdf->f(wi, wo) * Vector3::cos(hd.normal, wi) / d2;
                    rad += scene.objects[hd.obj_id]->phong_lighting(li, wi * -1, wo, hd.normal, d2);
                }
            }
        }
        if (debug && debug_ray) {
            std::cout << "direct rad " << rad << "\n";
        }

        return rad;
    }
    void Pathtracer::trace_light_ray(Path &vertices, Ray ray_in, Vector3 cumulative) {

        Vector3 wo, wi;
        double pdf;
        int bounce = 1;
        HitRecord hd;
        while (true) {

            if (bounce >= max_dl_bounce) {
                //ROUSSIAN ROULETTE to terminate path
                double p = cumulative.max() * terminate_param;
                if (drand48() >= p)
                    return;
                cumulative /= p;
            }

            if (!scene.find_intersection(ray_in, hd))
                break;
            wi = ray_in.get_direction();
            vertices.push_back(Vertex(cumulative, wi, hd));

            double cos_theta =  Vector3::cos(wi, hd.normal);
            //wo is now still in object space
            Vector3 color = scene.objects[hd.obj_id]->material->bsdf->sampleBSDF(wi, wo, pdf, cos_theta);

            //integration of Li * brdf * cos(wi, normal)
            //cos(normal, wo world) = wo.z in object space
            cumulative *= color * cos_theta/ pdf; //incoming light for the next vertices

            //transform wo from object space to world space
            Matrix3x3 o2w = Matrix3x3::modelToWorld(hd.normal);
            wo = o2w * wo;

            ray_in = Ray(hd.point, wo);
            ++bounce;
        }
    }
//
//    Paths Pathtracer::generate_light_path() {
//        double forwardPdf = 1.0;
//        double backwardPdf = 1.0;
//
//        Paths lightPaths;
//
//        for (auto light: scene.lights) {
//            Path path;
//
//            Ray ray = light->sampleLightRay(forwardPdf, backwardPdf);
//
//            HitRecord hd;
//            hd.point = ray.get_origin();
//            //forward pdf is sampling surface
//            Vertex v(light->material->ke / backwardPdf, hd);
//            path.push_back(v);
////            std::cout << "v.li/forwardPdf" << v.li/forwardPdf << "\n";
//            trace_light_ray(path, ray, v.li);
//            lightPaths.push_back(path);
//            if (debug && debug_ray) {
//                std::cout << "li of light " << v.li << "\n";
//                std::cout << "NB LIGHT POINTs per light " << path.size() << "\n";
//            }
//
//        }
//
//        return lightPaths;
//    }

//    Vector3 Pathtracer::trace(double x, double y) {
//
//        Paths lightPaths;
//        if (max_dl_bounce > 0)
//            lightPaths = generate_light_path();
//
//        if (debug && debug_ray) {
//            std::cout << "nb of light vertices: " << lightPaths.size() << "\n";
//            for (auto p: lightPaths)
//                std::cout << "nb of vertices in a light: " << p.size() << "\n";
//        }
//        Ray ray = scene.init_ray(x, y);
//        return get_radiance(ray, 0, lightPaths);
//    }
}
//    void Pathtracer::trace_light(Vertices& vertices,
//                                 const Ray &ray, const int bounces,
//                                 float scale)
//    {
//        if (debug && debug_ray)
//            std::cout << "TRACE LIGHT: " << ray << "; nth bounce: " << bounces << "\n";
//
//        double p = 1.0;
//        if (bounces >= max_dl_bounce)
//        {
//            //ROUSSIAN ROULETTE to terminate path
//            p = prev.col.max() * terminate_param;
//            if (drand48() >=  p)
//                return;
//        }
//        HitRecord hd; //hit data
//        if (!scene.find_intersection(ray, hd))
//            return;
//
//
//        if (hd.normal.dot(ray.get_direction()) > 0)
//            hd.normal *= -1;
//        shared_mat mat = scene.objects[hd.obj_id]->material;
//
//        //emissive color at this point ???
//        Vector3 col = scene.objects[hd.obj_id]->get_color_from_light_point(scene.objects, prev, hd,
//                                                                           ray.get_direction());
//        col *= scale;
//        col += mat->ke;
//
//        //col.clamp(0, max_intensity);
//        Vertex v(hd.point, hd.obj_id, col);
//        if (debug && debug_ray)
//            std::cout << "obj id: " << hd.obj_id <<"\n";
//        vertices.push_back(v);
//
//        Vector3 dir;
//        switch (mat->type) {
//            case Material::DIFFUSE:
//                dir = sample_diffuse(hd.normal);
//                break;
//            case Material::SPECULAR:
//                dir = ray.get_direction().reflect(hd.normal);
//                break;
//            default:
//                dir = sample_specular(hd.normal, mat->ns);
//        }
//        trace_light(vertices, v, Ray(hd.point, dir), bounces + 1, mat->kr);
//    }


