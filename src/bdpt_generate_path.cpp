#include "render.hh"
#include "utils/matrix.hh"


namespace pathtracing {
    std::vector<Vertex> Pathtracer::generate_eye_path(Ray ray) {
        std::vector<Vertex> path;

        Vector3 cumulative(1.0);
        double pdf;
        double backward_pdf_G = 1.0;
        double d2 = 1.0;
        int niter = 0;
        HitRecord hd;

        while (true) {
            if (!scene.find_intersection(ray, hd)) {
                break;
            }
            hd.normal.normalize();
            Matrix3x3 m2w = Matrix3x3::modelToWorld(hd.normal);
            Matrix3x3 w2m = m2w.transpose();
            BSDFRecord br;
            br.wi = w2m * ray.get_direction() * -1;

            shared_bsdf bsdf = scene.objects[hd.obj_id]->bsdf;
            shared_obj obj = scene.objects[hd.obj_id];

            if (niter >= max_idl_bounce) {
                //ROUSSIAN ROULETTE to terminate path
                double p = std::min(1.0, cumulative.max()) * terminate_param;
                if (drand48() >= p)
                    break;
                cumulative /= p;
            }

            if (niter > 0) //backward pdf * G
            {
                backward_pdf_G *= std::max(EPSILON, cos_theta(br.wi)); //G(i, i-1)
                d2 = (path[niter - 1].data.point - hd.point).norm_square();
            }

            cumulative *= bsdf->evalSampleBSDF(br, pdf);

            Vertex v(hd, cumulative, backward_pdf_G);
            v.dir = br.wi;
            v.d2 = d2;
            path.push_back(v);

            if (bsdf->is_light()) {
                Vector3 le = scene.objects[hd.obj_id]->Le(hd.point, br.wi);
                if (le.max() > EPSILON) {
////                    cumulative_pdf *= scene.objects[hd.obj_id]->sampleSurfacePositionPDF();
//                    Vertex v(hd, cumulative, cumulative_pdf);
//                    v.dir = br.wi;
//                    v.is_emissive = true;
//                    path.push_back(v);
                    break;
                }

            }

            backward_pdf_G = pdf * std::max(0.0, cos_theta(br.wo));
            br.wo = m2w * br.wo;
            ray = Ray(hd.point + br.wo * EPSILON, br.wo);
            ++niter;
        }

        return path;
    }


    std::vector<Vertex> Pathtracer::generate_light_path() {
        std::vector<Vertex> path;

        if (max_dl_bounce < 1)
            return path;

        //random select light from multiple light sources
        double rnd = drand48() * scene.lights.size();
        double select_light_pdf = 1.0 / scene.lights.size();
        unsigned int light_id = floor(rnd);
        auto light = scene.lights[light_id];

        //Add light point: the first vertex of path
        double backward_pdf_G;
        HitRecord hd;

        //sampling light ray position
        hd.point = light->sampleSurfacePosition(backward_pdf_G, hd.normal);
        hd.obj_id = light_id;
        backward_pdf_G *= select_light_pdf;
        Vector3 cumulative(1.0 / backward_pdf_G);

        Vertex v(hd, cumulative, backward_pdf_G);
        path.push_back(v);

        //sampling light ray direction
        double pdf;
        Vector3 dir = cosineSampleHemisphere(pdf);
        cumulative *= light->Le(hd.point, dir) * std::max(0.0, cos_theta(dir)) / pdf;
        backward_pdf_G = pdf * std::max(0.0, cos_theta(dir));

        Matrix3x3 m2w = Matrix3x3::modelToWorld(hd.normal);
        dir = m2w * dir;
        Ray ray(hd.point, dir);

        int niter = 1;

        BSDFRecord br;

        while (true) {
            if (!scene.find_intersection(ray, hd)) {
                break;
            }
            hd.normal.normalize();

            Matrix3x3 m2w = Matrix3x3::modelToWorld(hd.normal);
            Matrix3x3 w2m = m2w.transpose();
            shared_bsdf bsdf = scene.objects[hd.obj_id]->bsdf;

            //wi : from hit point to previous light vertex
            br.wi = w2m * ray.get_direction() * -1;


            if (bsdf->is_light()) { //le(x, theta_x)
                break;
            }

            if (niter >= max_dl_bounce) {
                //ROUSSIAN ROULETTE to terminate path
                double p = std::min(1.0, cumulative.max())  * terminate_param;
                if (drand48() >= p)
                    break;
                cumulative /= p;
            }


            backward_pdf_G *= std::max(EPSILON, cos_theta(br.wi));

            Vertex v(hd, cumulative, backward_pdf_G);
            v.dir = br.wi;
            v.d2 = (path[niter - 1].data.point - hd.point).norm_square();
            path.push_back(v);


            cumulative *= bsdf->evalSampleBSDF(br, pdf);


            backward_pdf_G = std::max(0.0, cos_theta(br.wo)) * pdf; //backward pdf

            br.wo = m2w * br.wo;
            ray = Ray(hd.point + br.wo * EPSILON, br.wo);
            ++niter;
        }
        return path;
    }
}
