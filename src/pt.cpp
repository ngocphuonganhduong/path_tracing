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
        Vector3 col;

        while (true) {
            if (!scene.find_intersection(ray, hd)) {
                break;
            }

            shared_bsdf bsdf = scene.objects[hd.obj_id]->bsdf;

            rad += scene.ambient_light * bsdf->ka() * cumulative;

            hd.normal.normalize();
            Matrix3x3 m2w = Matrix3x3::modelToWorld(hd.normal);
            Matrix3x3 w2m = m2w.transpose();

            BSDFRecord br;
            br.wi = w2m * (ray.get_direction().normalize()) * -1;

            if (bsdf->is_light())
            {
                light_pdf = scene.objects[hd.obj_id]->sampleSurfacePositionPDF();
                rad += scene.objects[hd.obj_id]->Le(hd.point, br.wi) * cumulative * bsdf_pdf / (bsdf_pdf + light_pdf);
            }


            for (auto light: scene.lights) {
                Vector3 lightNormal;
                Vector3 pos = light->sampleSurfacePosition(light_pdf, lightNormal);
                Vector3 l2o = hd.point - pos; //light to object
                double d2 = l2o.norm_square();
                l2o.normalize();
                HitRecord tem;
                if (scene.find_intersection(Ray(pos + l2o * EPSILON, l2o), tem) && tem.obj_id == hd.obj_id) {
                    br.wo = w2m * (l2o.normalize() * -1); //object to light
                    if (d2 < 1) // avoid too small
                        d2 = 1;
                    Matrix3x3 l2w = Matrix3x3::modelToWorld(lightNormal);
                    Matrix3x3 w2l = l2w.transpose();
                    Vector3 l2o_lightSpace = w2l * l2o;

                    Vector3 color = light->Le(pos, l2o_lightSpace) * bsdf->f(br, bsdf_pdf) * cumulative;
                    color *= std::max(0.0, cos_theta(br.wo)) * std::max(0.0, cos_theta(l2o_lightSpace));
                    color *= light->bsdf->attenuation(d2);
                    color /= (bsdf_pdf + light_pdf); //weights
                    rad += color;
                }
            }

            cumulative *= bsdf->evalSampleBSDF(br, bsdf_pdf); //obtain wo and pdf

            if (niter >= max_idl_bounce) {
                //ROUSSIAN ROULETTE to terminate path
                double p = cumulative.max() * terminate_param;
                if (drand48() >= p)
                    break;
                cumulative /= p;
            }

            br.wo = m2w * br.wo;
            ray = Ray(hd.point + br.wo * EPSILON, br.wo);

        }

        return rad;
    }
}