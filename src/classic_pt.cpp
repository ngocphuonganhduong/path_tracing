#include "render.hh"

namespace pathtracing {

    Vector3 Pathtracer::trace_classic_pt(Ray ray) {
        HitRecord hd;
        Vector3 rad;
        Vector3 cumulative(1.0);

        double bsdf_pdf = 1.0;
        int niter = 0;
        while (true) {
            if (!scene.find_intersection(ray, hd)) {
                break;
            }

            shared_mat mat = scene.objects[hd.obj_id]->material;
            rad += scene.ambient_light * mat->ka * cumulative;

            if (mat->ke.max() > 0) {
                rad += mat->ke * cumulative;
            }

            if (niter >= max_idl_bounce) {
                //ROUSSIAN ROULETTE to terminate path
                double p = cumulative.max() * terminate_param;
                if (drand48() >= p)
                    break;
                cumulative /= p;
            }

            hd.normal.normalize();
            Matrix3x3 m2w = Matrix3x3::modelToWorld(hd.normal);
            Matrix3x3 w2m = m2w.transpose();
            BSDFRecord br;

            br.wi = w2m * (ray.get_direction()) * -1;

            Vector3 f = mat->bsdf->sampleBSDF(br, bsdf_pdf); //obtain wo and pdf

//            if (debug_ray && debug) {
//                std::cout << cumulative << " f:" << f << " pdf:" << bsdf_pdf << "\n";
//                std::cout << "wo " << br.wo << " wi" << br.wi << "\n";
//                std::cout << "w2m " << w2m << " m2w" << m2w << "\n";
//                std::cout << "rad " << rad << "\n";
//            }
            cumulative *= f * fabs(cos_theta(br.wo)) / bsdf_pdf;

            rad.clamp(0.0, 1.0);
            ray = Ray(hd.point, m2w * br.wo);
        }
        return rad;
    }

}