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

        //case i=-1 (no light vertex): path tracing
        //case
        //
        //estimator = sum of Wij * Cij
        Vector3 rad(0.0);
        std::vector<Vertex> eye_path = generate_eye_path(ray); //not contain camera point
        if (eye_path.size() < 1) //we dont implement classic light tracer because of our pinhole camera
            return rad;

        std::vector<Vertex> light_path = generate_light_path();

        HitRecord hd;
        Vector3 c; //contribution
        Vector3 w; //weight
        int i;

        int ep_size = eye_path.size();
        int lp_size = light_path.size();
        double pdf;
        BSDFRecord br;

        int max_path_length = ep_size + lp_size;

        double d2;
        double path_pdf;
        double sum_path_pdf;
        Vector3 s_rad;
        //max_path_length: count from y0
#ifdef DEBUG_RAY
        std::string str;
        str += "Light p: " + std::to_string(lp_size) + "  Eye P: " + std::to_string(ep_size) + "\n";
#endif

        for (int s = 0; s <= max_path_length; ++s) {
#ifdef DEBUG_RAY
            str += "s=" + std::to_string(s) + "\n";
#endif

            //case: i=-1, j > 0: classic pathtracer
            //s=4: //y3-y2-y1-y0-camera


            //classic path tracer: i=-1
            //bdpt
            sum_path_pdf = 0;
            s_rad = Vector3(0.0);
            for (int j = 0; j <= s; ++j) { //eye
                i = s - j - 1; //
                //ex: s=0: (i,j)=-1, 0;
                //ex: s=1: (i,j)=(0, 1)
                //ex: s=4, (i,j)=(2,0) x0-x1-x2-y0
                //ex: s=4, (i,j)=(1,1) x0-x1-y1-y0
                //ex: s=4, (i,j)=(0,2) x0-y2-y1-y0
                //ex: s=4, (i,j)=(-1,3) y3-y2-y1-y0-camera //classic pathtracer
                path_pdf = 1.0;

                if (j >= ep_size || i >= lp_size)
                    continue;
#ifdef DEBUG_RAY
                str += "(i, j) = (" + std::to_string(i) + "," + std::to_string(j) + ")\n";
#endif

                auto ev = eye_path[j];

                if (i == -1) //s=j, classic pathtracer
                {
                    if (j == ep_size - 1) {
                        c = ev.cumulative;
                        path_pdf *= ev.cumulative_pdf;
#ifdef DEBUG_RAY
                        str += " Classic PT hit light: (" + std::to_string(ev.cumulative.x()) + ","
                               + std::to_string(ev.cumulative.y()) + "," + std::to_string(ev.cumulative.z())
                               + " pdf: " + std::to_string(ev.cumulative_pdf) + "\n";
#endif

                    } else { //did not hit a light source
                        continue;
                    }
                } else {
#ifdef DEBUG_RAY
                    str += "BDPT \n";
#endif
                    auto lv = light_path[i];
                    Vector3 l2e = ev.data.point - lv.data.point;
                    d2 = l2e.norm_square();
                    l2e.normalize();
                    if (!scene.find_intersection(Ray(lv.data.point + l2e * EPSILON, l2e), hd) ||
                        hd.obj_id != ev.data.obj_id) {
                        continue;
                    }


                    Matrix3x3 l2w = Matrix3x3::modelToWorld(lv.data.normal);
                    Matrix3x3 w2l = l2w.transpose();
                    br.wo = w2l * l2e;

                    auto light = scene.lights[0];
                    //light path
                    if (i == 0) { //pathtracer with direct illumination
                        c = light->Le(lv.data.point, br.wo);
                        if (c.max() < EPSILON)
                            continue;
#ifdef DEBUG_RAY
                        str += "i=0: c: " + c.to_string() + " pdf: " + std::to_string(lv.cumulative_pdf) + "\n";
#endif


                        path_pdf *= lv.cumulative_pdf;
                    } else {

                        c = light_path[i - 1].cumulative;
#ifdef DEBUG_RAY
                        str += "i>0: c: " + c.to_string() + " cum:" + light_path[i - 1].cumulative.to_string() +
                               " pdf: " + std::to_string(light_path[i - 1].cumulative_pdf) + "\n";
#endif

                        path_pdf *= light_path[i - 1].cumulative_pdf;
                        //light contribution bwt xi and yj
                        br.wi = lv.dir; //lv.dir in light space
                        c *= scene.objects[lv.data.obj_id]->bsdf->brdf(br,
                                                                       pdf); //pdf here dont have to use since we dont sample ray bwt xi and yj
                    }

#ifdef DEBUG_RAY
                    str += "C before eye " + c.to_string() + "\n";
#endif

                    //eye path
                    if (j > 1) {
                        c *= eye_path[j - 1].cumulative;
                        path_pdf *= eye_path[j - 1].cumulative_pdf;
                    }
#ifdef DEBUG_RAY
                    str += "C  after eye " + c.to_string() + "\n";
#endif


                    Matrix3x3 e2w = Matrix3x3::modelToWorld(ev.data.normal);
                    Matrix3x3 w2e = e2w.transpose();
                    br.wo = ev.dir; //ev.dir in light space;
                    br.wi = w2e * l2e * -1;
#ifdef DEBUG_RAY
                    str += "eye  " + scene.objects[ev.data.obj_id]->bsdf->brdf(br, pdf).to_string() + "\n";
#endif

                    c *= scene.objects[ev.data.obj_id]->bsdf->brdf(br, pdf) * light->bsdf->attenuation(d2);

#ifdef DEBUG_RAY
                    str += "C  " + c.to_string() + "\n";
#endif


                }
#ifdef DEBUG_RAY
                str += "C path " + c.to_string() + " ;  path_pdf: " + std::to_string(path_pdf) + "\n";
#endif

                s_rad += c * path_pdf;
                sum_path_pdf += path_pdf;

            }
            if (s_rad.max() > 0) {
#ifdef DEBUG_RAY
                str += "s_rad " + s_rad.to_string() + " ;  sum pdf: " + std::to_string(sum_path_pdf) + "\n";
                str += "RAD E: " + (s_rad / sum_path_pdf).to_string() + "\n";
#endif
                rad += s_rad / sum_path_pdf;

            }
        }

#ifdef DEBUG_RAY
        if (rad.max() > 50) {
            std::cout << " FIREFLIESssssssssssssssssssssssssssssssss " << rad << "\n";
            std::cout << str << "\n\n";
        }
#endif


        return rad;
    }


    std::vector<Vertex> Pathtracer::generate_eye_path(Ray ray) {
        Vector3 rad;
        std::vector<Vertex> path;

        Vector3 cumulative(1.0);
        double pdf;
        double cumulative_pdf = 1.0;
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
            shared_bsdf bsdf = scene.objects[hd.obj_id]->bsdf;
            shared_obj obj = scene.objects[hd.obj_id];

            //wi : from hit point to previous light vertex
            br.wi = w2m * ray.get_direction() * -1;


            //AMBIENT
//            rad += scene.ambient_light * bsdf->ka() * cumulative;


            if (bsdf->is_light()) {
                Vertex v(hd, scene.objects[hd.obj_id]->Le(hd.point, br.wi) * cumulative, cumulative_pdf);
                v.dir = br.wi;
                path.push_back(v);
                break;
            }

            if (niter >= max_idl_bounce) {
                //ROUSSIAN ROULETTE to terminate path
                double p = cumulative.max() * terminate_param;
                if (drand48() >= p)
                    break;
                cumulative /= p;
                cumulative_pdf *= p;
            }

            //f * cos(wo, hd.normal)
            cumulative *= bsdf->sampleBRDF(br, pdf);
            cumulative *= fabs(cos_theta(br.wo)) / pdf;
            cumulative_pdf *= pdf;

            Vertex v(hd, cumulative, cumulative_pdf);
            v.dir = ray.get_direction() * -1;
            path.push_back(v);

            ray = Ray(hd.point, m2w * br.wo);


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
        double cumulative_pdf;
        HitRecord hd;

        //sampling light ray position
        hd.point = light->sampleSurfacePosition(cumulative_pdf, hd.normal);
        hd.obj_id = light_id;
        cumulative_pdf *= select_light_pdf;
        Vector3 cumulative(1.0 / cumulative_pdf);

        Vertex v(hd, cumulative, cumulative_pdf);
        path.push_back(v);

        //sampling light ray direction
        double pdf;
        Vector3 dir = cosineSampleHemisphere(pdf);
        cumulative *= light->Le(hd.point, dir) * std::max(0.0, cos_theta(dir))/pdf;
        cumulative_pdf *= pdf;


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
            br.wi = w2m * v.dir;

            //AMBIENT
//            rad += scene.ambient_light * bsdf->ka() * cumulative;

            if (bsdf->is_light()) { //le(x, theta_x)
                break;
            }

            if (niter >= max_dl_bounce) {
                //ROUSSIAN ROULETTE to terminate path
                double p = cumulative.max() * terminate_param;
                if (drand48() >= p)
                    break;
                cumulative /= p;
                cumulative_pdf *= p;
            }



            //f * cos(wo, hd.normal)
            cumulative *= bsdf->sampleBRDF(br, pdf);
            cumulative *= fabs(cos_theta(br.wo)) / pdf;
            cumulative_pdf *= pdf;

            Vertex v(hd, cumulative, cumulative_pdf);
            v.dir = br.wi;
            path.push_back(v);

            ray = Ray(hd.point, m2w * br.wo);
            ++niter;
        }
        return path;
    }

}