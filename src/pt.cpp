#include "render.hh"

#define W_BIAS 5.0

namespace pathtracing {

    Vector3  Pathtracer::visibility_test(const Vector3& position, const Vector3& direction, unsigned int obj_id) const{
        HitRecord tem;
        Vector3 filter_opacity(1.0);
        tem.point = position;
        while (true) {
            if (!scene.find_intersection(Ray(tem.point + direction * EPSILON, direction), tem)) {
                return Vector3(0.0);
            }
            if (tem.obj_id == obj_id) {
                break;
            } else {
                filter_opacity *= scene.objects[tem.obj_id]->bsdf->filter_opacity();
            }
            if (filter_opacity.max() < EPSILON)
                break;
        }
        return filter_opacity;
    }

    Vector3 Pathtracer::trace_pt(Ray ray) {
        HitRecord hd;
        Vector3 rad;
        Vector3 cumulative(1.0);
        double bsdf_pdf = 1.0;
        double light_pdf = 0.0;
        double d2;
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
                if (niter > 0) {
                    d2 = (hd.point - ray.get_origin()).norm_square();
                    light_pdf = scene.objects[hd.obj_id]->sampleSurfacePositionPDF();
                    light_pdf /= scene.objects[hd.obj_id]->bsdf->attenuation(d2) * std::max(EPSILON, cos_theta(br.wi));//* scene.objects[hd.obj_id]->bsdf->attenuation(d2);
                    rad += scene.objects[hd.obj_id]->Le(hd.point, br.wi) * cumulative * W_BIAS * bsdf_pdf /
                           (W_BIAS * bsdf_pdf + light_pdf);
                } else {
                    rad += scene.objects[hd.obj_id]->Le(hd.point, br.wi);
                }
            }

            for (auto light: scene.lights) {
                Vector3 lightNormal;
                Vector3 pos = light->sampleSurfacePosition(light_pdf, lightNormal);
                Vector3 l2o = hd.point - pos; //light to object
                d2 = l2o.norm_square();
                l2o.normalize();
                Vector3 visibility = visibility_test(pos, l2o, hd.obj_id);

                if (visibility.max() > EPSILON) {
                    br.wo = w2m * (l2o.normalize() * -1); //object to light

                    Matrix3x3 w2l = Matrix3x3::worldToModel(lightNormal);
                    Vector3 l2o_lightSpace = w2l * l2o;

                    light_pdf /= std::max(EPSILON, cos_theta(l2o_lightSpace)) * light->bsdf->attenuation(d2);
                    visibility *= light->Le(pos, l2o_lightSpace) * bsdf->evalBSDF(br, bsdf_pdf) * std::max(0.0, cos_theta(br.wo)) *
                            cumulative;

                    //convert to pw = pa / G()
                    visibility /= (W_BIAS * bsdf_pdf + light_pdf); //weights
                    rad += visibility;
                }
            }

            cumulative *= bsdf->evalSampleBSDF(br, bsdf_pdf); //obtain wo and pdf

            if (niter >= max_idl_bounce) {
                //ROUSSIAN ROULETTE to terminate path
                double p = cumulative.max() * terminate_param;
                if (drand48() >= p)
                    break;
                cumulative /= std::min(1.0, p);
            }

            br.wo = m2w * br.wo;

            ray = Ray(hd.point + br.wo * EPSILON, br.wo);
            ++niter;
        }

        return rad;
    }
}