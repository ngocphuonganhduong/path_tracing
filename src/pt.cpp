#include "render.hh"

namespace pathtracing {


    Vector3 Pathtracer::trace_pt(Ray ray) {
        HitRecord hd;
        Vector3 rad;
        Vector3 cumulative(1.0);
        double bsdf_pdf = 1.0;
        double light_pdf = 0.0;
        int niter = 0;

        while (true) {
            if (!scene.find_intersection(ray, hd)) {
                break;
            }
            shared_bsdf bsdf = scene.objects[hd.obj_id]->bsdf;

            //AMBIENT
            if (niter == 0)
                rad += scene.ambient_light * bsdf->ka() * cumulative * bsdf_pdf;
            else
                rad += scene.ambient_light * bsdf->ka() * cumulative * (bsdf_pdf - 1);

            hd.normal.normalize();
            Matrix3x3 m2w = Matrix3x3::modelToWorld(hd.normal);
            Matrix3x3 w2m = m2w.transpose();

            BSDFRecord br;
            br.wi = w2m * (ray.get_direction().normalize()) * -1;

            if (bsdf->is_light()) {
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
                Vector3 filter_opacity(1.0);
                tem.point = pos;
                while (true) {
                    if (!scene.find_intersection(Ray(tem.point + l2o * EPSILON, l2o), tem)) {
                        filter_opacity = Vector3(0.0);
                        break;
                    }
                    if (tem.obj_id == hd.obj_id) {
                        break;
                    } else {
                        filter_opacity *= scene.objects[tem.obj_id]->bsdf->filter_opacity();
                    }
                    if (filter_opacity.max() < EPSILON)
                        break;
                }
                if (filter_opacity.max() > EPSILON) {
                    br.wo = w2m * (l2o.normalize() * -1); //object to light
                    if (d2 < 1) // avoid too small
                        d2 = 1;
                    Matrix3x3 l2w = Matrix3x3::modelToWorld(lightNormal);
                    Matrix3x3 w2l = l2w.transpose();
                    Vector3 l2o_lightSpace = w2l * l2o;

                    filter_opacity *= light->Le(pos, l2o_lightSpace) * bsdf->f(br, bsdf_pdf) * cumulative;
                    filter_opacity *= std::max(0.0, cos_theta(br.wo)) * std::max(0.0, cos_theta(l2o_lightSpace));
                    filter_opacity *= light->bsdf->attenuation(d2);
                    filter_opacity /= (bsdf_pdf + light_pdf); //weights
                    rad += filter_opacity;
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
            ++niter;
        }

        return rad;
    }
}