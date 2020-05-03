#include "render.hh"
#include "utils/matrix.hh"

namespace pathtracing {
    Vector3 Pathtracer::trace_bdpt(Ray ray) {
        //x0 -> x1 -> ... -> xi -> yj -> ... y1 -> y0
        //x0: light source
        //y0: first intersection with from camera to scene
        //skip case light tracing until hit camera aperture, which is <<)
        //(skip case j=-1, j=0 in the subject, NOT implement classic light tracing because the current camera is a pinhole camera)
        //It's impossible for a random light ray from a light source to hit a point TT

        //ESTIMATOR = sum of Wij * Cij
        Vector3 rad(0.0);
        std::vector<Vertex> eye_path = generate_eye_path(ray); //not contain camera point
        if (eye_path.size() < 1) //we dont implement classic light tracer because of our pinhole camera
            return rad;

        std::vector<Vertex> light_path = generate_light_path();

        HitRecord hd;
        Vector3 c; //contribution

        int ep_size = eye_path.size();
        int lp_size = light_path.size();
        double pdf;
        BSDFRecord br;

        //max_path_length: COUNT from y0 = number of vertices in path(excluding camera)
        int max_path_length = ep_size + lp_size;

        double d2;
        double sum_weight;
        Vector3 sum_rad;
#ifdef DEBUG_RAY
        std::string str;
        str += "Light p: " + std::to_string(lp_size) + "  Eye P: " + std::to_string(ep_size) + "\n";
#endif
        //s=0: (i,j): (-1, 0) : y0
        rad += scene.objects[eye_path[0].data.obj_id]->Le(eye_path[0].data.point, eye_path[0].dir);
        auto light = scene.lights[light_path[0].data.obj_id];
//        auto ev = eye_path[0];
//        int il = 2;
//        if (lp_size <= il)
//            il = lp_size - 1;
//        auto lv = light_path[il];
//        Vector3 l2e = ev.data.point - lv.data.point;
//        d2 = l2e.norm_square();
//        if (d2 < 1)
//            return rad;
//        l2e.normalize();
//
//
//        if (scene.find_intersection(Ray(lv.data.point + l2e * EPSILON, l2e), hd) &&
//            hd.obj_id == ev.data.obj_id) {
//
//            Matrix3x3 l2w = Matrix3x3::modelToWorld(lv.data.normal);
//            Matrix3x3 w2l = l2w.transpose();
//            br.wo = w2l * l2e;
//
//            if (il == 0)
//                c = light->Le(lv.data.point, br.wo) * lv.cumulative;
//            else {
//                c = lv.cumulative;
//                br.wi = lv.dir; //lv.dir in light space
//                c *= scene.objects[lv.data.obj_id]->bsdf->brdf(br, pdf);
//
//            }
//
//            c *= std::max(0.0, cos_theta(br.wo));
//
//            Matrix3x3 e2w = Matrix3x3::modelToWorld(ev.data.normal);
//            Matrix3x3 w2e = e2w.transpose();
//            br.wi = ev.dir; //ev.dir in eye space;
//            br.wo = w2e * l2e * -1;
//            c *= std::max(0.0, cos_theta(br.wo));
//            c *= scene.objects[ev.data.obj_id]->bsdf->brdf(br, pdf) * light->bsdf->attenuation(d2);
////            if (c.max() > 5)
////                std::cout << c << "\n";
//
//            c.clamp(0.0, 1);
//            return c;
//        }
//        return rad;

        for (int s = 1; s < max_path_length; ++s) {
#ifdef DEBUG_RAY
            str += "s=" + std::to_string(s) + "\n";
#endif

            std::vector<double> mis_ratios;
            for (int i = 0; i < lp_size - 1; ++i) { //loop over light vertices
                int ev_id = s - i - 1;
                if (ev_id < 0)
                    break;
                if (ev_id >= ep_size)
                    continue;
                double ratio = eye_path[ev_id].backward_pdf_G * light->bsdf->attenuation(eye_path[ev_id].d2);
                ratio /= light_path[i + 1].backward_pdf_G * light->bsdf->attenuation(light_path[i + 1].d2);
#ifdef DEBUG_RAY
                str += "eye pdfG: " + std::to_string(eye_path[ev_id].backward_pdf_G) + "; light pdfG: " + std::to_string(light_path[i + 1].backward_pdf_G) + "\n";

                str +="i: " + std::to_string(i) +  " ; ev_id: " + std::to_string(ev_id) + " ; lv_id: " + std::to_string(i + 1) + " ; ratio: " + std::to_string(ratio) + "\n";
#endif
                mis_ratios.push_back(ratio);
            }

            const int mis_size = mis_ratios.size();
#ifdef DEBUG_RAY
            str += "mis_size: " + std::to_string(mis_size) + "\n";
#endif

            sum_rad = Vector3(0.0);

            for (int j = 0; j <= s; ++j) { //eye
                int i = s - j - 1; //

                //ex: s=1: (i,j): (0, 0)  : x0-y0

                //ex: s=2: (i,j): (1, 0)  : x0-x1-y0
                //ex: s=2: (i,j): (0, 1)  : x0-y1-y0

                //ex: s=3, (i,j)=(2,0) x0-x1-x2-y0
                //ex: s=3, (i,j)=(1,1) x0-x1-y1-y0
                //ex: s=3, (i,j)=(0,2) x0-y2-y1-y0

                if (j >= ep_size || i >= lp_size || i < 0)
                    continue;
#ifdef DEBUG_RAY
                str += "(i, j) = (" + std::to_string(i) + "," + std::to_string(j) + ")\n";
#endif

                auto ev = eye_path[j];
                auto lv = light_path[i];
                Vector3 l2e = ev.data.point - lv.data.point;
                d2 = l2e.norm_square();
                l2e.normalize();

                //occlusion test: shoot shadow ray
                c = Vector3(0.0);
                if (scene.find_intersection(Ray(lv.data.point + l2e * EPSILON, l2e), hd) &&
                    hd.obj_id == ev.data.obj_id) {

                    Matrix3x3 l2w = Matrix3x3::modelToWorld(lv.data.normal);
                    Matrix3x3 w2l = l2w.transpose();
                    br.wo = w2l * l2e;

                    //LIGHT path contribution
                    if (i == 0) { //classic pathtracer
                        //since i == 0: we need to re-evaluate the contribution Le(xo, xi->yj)
                        c = light->Le(lv.data.point, br.wo) * lv.cumulative;
                        if (c.max() < EPSILON)
                            continue;
#ifdef DEBUG_RAY
                        str += "i=0: c: " + c.to_string() + "\n";
#endif
                    } else {
                        c = lv.cumulative;

#ifdef DEBUG_RAY
                        str += "i>0: c: " + c.to_string() + " cum:" + light_path[i - 1].cumulative.to_string() + "\n";
#endif
                        //amount of light contribution from xi to yj is the same
                        br.wi = lv.dir; //lv.dir in light space
                        c *= scene.objects[lv.data.obj_id]->bsdf->brdf(br, pdf);
                    }

#ifdef DEBUG_RAY
                    str += "C before eye " + c.to_string() + "\n";
#endif

//                    EYE path contribution
                    if (j > 0) { //from y0 to yj-1
                        c *= eye_path[j - 1].cumulative;
                    }

#ifdef DEBUG_RAY
                    str += "C after eye " + c.to_string() + "\n";
#endif
                    c *= std::max(0.0, cos_theta(br.wo));

                    Matrix3x3 e2w = Matrix3x3::modelToWorld(ev.data.normal);
                    Matrix3x3 w2e = e2w.transpose();
                    br.wo = ev.dir; //ev.dir in eye space;
                    br.wi = w2e * l2e * -1;
                    c *= std::max(0.0, cos_theta(br.wi));
#ifdef DEBUG_RAY
                    str += "C: " + c.to_string() + "\n";
#endif

                    c *= scene.objects[ev.data.obj_id]->bsdf->brdf(br, pdf) * light->bsdf->attenuation(d2);
#ifdef DEBUG_RAY
                    str += "C final " + c.to_string() + " ; f:" +  scene.objects[ev.data.obj_id]->bsdf->brdf(br, pdf).to_string() + "\n";
#endif

                } else
                    continue;

                sum_weight = 1.0; //used for evaluate
                double ratio = 1.0;
                for (int mis_id = 0; mis_id < i && mis_id < mis_size; ++mis_id) //loop from  0th lv to i - 1 lv
                {
                    ratio *= mis_ratios[mis_id];
                    sum_weight += ratio * ratio;
#ifdef DEBUG_RAY
                    str += "mis_id <: " +  std::to_string(mis_id) + " ratio square: " + std::to_string(ratio * ratio) +  "\n";
#endif

                }

                ratio = 1.0;
                for (int mis_id = i + 1; mis_id < mis_size; ++mis_id) //loop from  i + 1 lv to  lv_size
                {
                    ratio *= mis_ratios[mis_id];
                    sum_weight += 1.0 / (ratio * ratio);
#ifdef DEBUG_RAY
                    str += "mis_id> : " +  std::to_string(mis_id) + " ratio square: " + std::to_string(1.0/(ratio * ratio)) +  "\n";
#endif

                }
                c /= sum_weight;

                sum_rad += c;
#ifdef DEBUG_RAY
                str += "sum_rad: " +  sum_rad.to_string() + " ; sum_weight: " + std::to_string(sum_weight) + " ; c/sum_weight= " + (c/sum_weight).to_string() +  "\n";
#endif

            }

            rad += sum_rad;
        }
#ifdef DEBUG_RAY
        if (rad.max() > 10)
        {
            std::cout << "FIREFLIESSSSS " << rad.to_string() << "\n";
            std::cout << str << "\n\n";

        }
#endif

        return rad;
    }
}