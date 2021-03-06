#include "render.hh"

namespace pathtracing {

    Vector3 Pathtracer::trace_classic_pt(Ray ray) {
        HitRecord hd;
        Vector3 rad;
        Vector3 cumulative(1.0);
        double bsdf_pdf;
        BSDFRecord br;
        int niter = 0;

        while (true) {
            if (!scene.find_intersection(ray, hd)) {
                break;
            }

            hd.normal.normalize();
            Matrix3x3 m2w = Matrix3x3::modelToWorld(hd.normal);
            Matrix3x3 w2m = m2w.transpose();

            //wi : from hit point to eye vertex
            br.wi = w2m * (ray.get_direction()) * -1;
            shared_bsdf bsdf = scene.objects[hd.obj_id]->bsdf;

            if (bsdf->is_light()) { //le(x, theta_x)
                rad += scene.objects[hd.obj_id]->Le(hd.point, br.wi) * cumulative;
            }

            if (niter >= max_idl_bounce) {
                //ROUSSIAN ROULETTE to terminate path
                double p = cumulative.max() * terminate_param;
                if (drand48() >= p)
                    break;
                cumulative /= std::min(1.0, p);
            }

            cumulative *= bsdf->evalSampleBSDF(br, bsdf_pdf);

            ray = Ray(hd.point, m2w * br.wo);
            ++niter;
        }
        return rad;
    }

}