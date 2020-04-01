#include "render.hh"

namespace pathtracing {


    Vector3 Pathtracer::trace_pt(Ray ray) {
        HitRecord hd;
        Vector3 rad;
        Vector3 cumulative(1.0);
//        if (debug && debug_ray)
//            std::cout << "trace" << ray << " cumulative: " << cumulative << "\n";
        double bsdf_pdf = 1.0;
        double light_pdf = 0.0;
        int niter = 0;
        while (true) {
            if (!scene.find_intersection(ray, hd)) {
                break;
            }

            shared_mat mat = scene.objects[hd.obj_id]->material;

            rad += scene.ambient_light * mat->ka * cumulative;
            hd.normal.normalize();
            Matrix3x3 m2w = Matrix3x3::modelToWorld(hd.normal);
            Matrix3x3 w2m = m2w.transpose();

            BSDFRecord br;
            br.wi = w2m * (ray.get_direction().normalize()) * -1;

            if (mat->ke.max() > 0) {
                light_pdf = scene.objects[hd.obj_id]->sampleSurfacePositionPDF();
                rad += mat->ke * cumulative * bsdf_pdf / (bsdf_pdf + light_pdf);
            }

            for (auto light: scene.lights) {
                Vector3 pos = light->sampleSurfacePosition(light_pdf);
                Vector3 l2o = hd.point - pos; //light to object
                double d2 = l2o.norm_square();
                l2o.normalize();
                HitRecord tem;
                if (scene.find_intersection(Ray(pos + l2o * EPSILON, l2o), tem) && tem.obj_id == hd.obj_id) {
                    br.wo = w2m * (l2o.normalize() * -1); //object to light
                    if (d2 < 1) // avoid too small
                        d2 = 1;
                    Vector3 color = mat->bsdf->f(br) * fabs(cos_theta(br.wo)) * cumulative /
                                    ((bsdf_pdf + light_pdf) * (d2 * mat->a + sqrt(d2) + mat->b + mat->c));
                    rad += color;
                }
            }


            Vector3 f = mat->bsdf->sampleBSDF(br, bsdf_pdf); //obtain wo and pdf

            if (niter >= max_idl_bounce) {
                //ROUSSIAN ROULETTE to terminate path
                double p = cumulative.max() * terminate_param;
                if (drand48() >= p)
                    break;
                cumulative /= p;
            }
            cumulative *= f * fabs(cos_theta(br.wo)) / bsdf_pdf;

            ray = Ray(hd.point, m2w * br.wo);
        }
        return rad;
    }
}