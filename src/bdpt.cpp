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
        int i;

        int ep_size = eye_path.size();
        int lp_size = light_path.size();
        double pdf;
        BSDFRecord br;

        //max_path_length: COUNT from y0 = number of vertices in path(excluding camera)
        int max_path_length = ep_size + lp_size;

        double d2;
        double path_pdf;
        double sum_path_pdf;
        Vector3 s_rad;
#ifdef DEBUG_RAY
        std::string str;
        str += "Light p: " + std::to_string(lp_size) + "  Eye P: " + std::to_string(ep_size) + "\n";
#endif


        for (int s = 0; s < max_path_length; ++s) {
#ifdef DEBUG_RAY
            str += "s=" + std::to_string(s) + "\n";
#endif
            sum_path_pdf = 0; //used for evaluate
            s_rad = Vector3(0.0);
            for (int j = 0; j <= s; ++j) { //eye
                i = s - j - 1; //
                //ex: s=0: (i,j): (-1, 0) : y0

                //ex: s=1: (i,j): (0, 0)  : x0-y0
                //ex: s=1: (i,j): (-1, 1) : y1-y0

                //ex: s=2: (i,j): (1, 0)  : x0-x1-y0
                //ex: s=2: (i,j): (0, 1)  : x0-y1-y0
                //ex: s=2: (i,j): (-1, 2) : y2-y1-y0

                //ex: s=3, (i,j)=(2,0) x0-x1-x2-y0
                //ex: s=3, (i,j)=(1,1) x0-x1-y1-y0
                //ex: s=3, (i,j)=(0,2) x0-y2-y1-y0
                //ex: s=3, (i,j)=(-1,3) x0-y2-y1-y0

                path_pdf = 1.0;

                if (j >= ep_size || i >= lp_size)
                    continue;
#ifdef DEBUG_RAY
                str += "(i, j) = (" + std::to_string(i) + "," + std::to_string(j) + ")\n";
#endif

                auto ev = eye_path[j];

                if (i == -1) //s=j, classic pathtracer
                {
                    //ex: s=0: (i,j): (-1, 0) : y0
                    //ex: s=1: (i,j): (-1, 1) : y1-y0
                    //ex: s=2: (i,j): (-1, 2) : y2-y1-y0
                    //since eye path terminates when it is hit a light or exceeds the boundary
                    //contribution = 0 if j is not the last vertex of eye path or j is not light
                    if (ev.is_emissive) {
                        c = ev.cumulative;
                        path_pdf *= ev.cumulative_pdf;
#ifdef DEBUG_RAY
                        str += " Classic PT hit light: (" + std::to_string(ev.cumulative.x()) + ","
                               + std::to_string(ev.cumulative.y()) + "," + std::to_string(ev.cumulative.z())
                               + " pdf: " + std::to_string(ev.cumulative_pdf) + "\n";
#endif
                    } else //did not hit a light
                        continue;
                } else //case when i >= 0
                {
                    auto lv = light_path[i];
                    Vector3 l2e = ev.data.point - lv.data.point;
                    d2 = l2e.norm_square();
//                    if (d2 < 1) // avoid too small
//                        d2 = 1;
                    l2e.normalize();
                    //occlusion test: shoot shadow ray
                    if (!scene.find_intersection(Ray(lv.data.point + l2e * EPSILON, l2e), hd) ||
                        hd.obj_id != ev.data.obj_id) {
                        continue;
                    }


                    Matrix3x3 l2w = Matrix3x3::modelToWorld(lv.data.normal);
                    Matrix3x3 w2l = l2w.transpose();
                    br.wo = w2l * l2e;

                    auto light = scene.lights[light_path[0].data.obj_id];


                    //LIGHT path contribution
                    if (i == 0) { //pathtracer with direct illumination
                        //since i == 0: we need to re-evaluate the contribution Le(xo, xi->yj)
                        c = light->Le(lv.data.point, br.wo) * lv.cumulative;
                        if (c.max() < EPSILON)
                            continue;
#ifdef DEBUG_RAY
                        str += "i=0: c: " + c.to_string() + " pdf: " + std::to_string(lv.cumulative_pdf) + "\n";
#endif

                        path_pdf *= lv.cumulative_pdf;
                    } else {
//                        continue;
                        //since i != 0: contribution of light source is always Le(xo, xo->x1)
                        //amount of contribution from x0 to xi-1 is the same
                        c = light_path[i - 1].cumulative;
                        path_pdf *= light_path[i - 1].cumulative_pdf;

#ifdef DEBUG_RAY
                        str += "i>0: c: " + c.to_string() + " cum:" + light_path[i - 1].cumulative.to_string() +
                               " pdf: " + std::to_string(light_path[i - 1].cumulative_pdf) + "\n";
#endif
                        //amount of light contribution from xi to yj is the same
                        br.wi = lv.dir; //lv.dir in light space
                        c *= scene.objects[lv.data.obj_id]->bsdf->brdf(br, pdf);
#ifdef DEBUG_RAY
                        str += "wo: " + br.wo.to_string() + " ; pdf: " + std::to_string(pdf) + " ; path_pdf: " +
                               std::to_string(path_pdf) + "\n";
#endif

                    }

#ifdef DEBUG_RAY
                    str += "C before eye " + c.to_string() + "\n";
#endif

//                    EYE path contribution
                    if (j > 1) { //from y0 to yj-1
                        c *= eye_path[j - 1].cumulative;
                        path_pdf *= eye_path[j - 1].cumulative_pdf;
                    }
#ifdef DEBUG_RAY
                    str += "C  after eye " + c.to_string() + "\n";
#endif
                    c *= std::max(0.0, cos_theta(br.wo));

                    Matrix3x3 e2w = Matrix3x3::modelToWorld(ev.data.normal);
                    Matrix3x3 w2e = e2w.transpose();
                    br.wo = ev.dir; //ev.dir in eye space;
                    br.wi = w2e * l2e * -1;
                    c *= std::max(0.0, cos_theta(br.wi));

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
        if (rad.max() > 10) {
            std::cout << " FIREFLIESssssssssssssssssssssssssssssssss " << rad << "\n";
            std::cout << str << "\n\n";
        }
#endif

        return rad;
    }

    std::vector<Vertex> Pathtracer::generate_eye_path(Ray ray) {
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

            //br.wi : from hit point to eye vertex
            br.wi = w2m * ray.get_direction() * -1;

            shared_bsdf bsdf = scene.objects[hd.obj_id]->bsdf;
            shared_obj obj = scene.objects[hd.obj_id];

            //AMBIENT
//            rad += scene.ambient_light * bsdf->ka() * cumulative;

            if (bsdf->is_light()) {
                Vector3 le = scene.objects[hd.obj_id]->Le(hd.point, br.wi);
                if (le.max() > EPSILON) {
                    Vertex v(hd, le * cumulative, cumulative_pdf);
                    v.dir = br.wi;
                    v.is_emissive = true;
                    path.push_back(v);
                }
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
            cumulative *= bsdf->evalSampleBSDF(br, pdf);
            cumulative_pdf *= pdf;


            ray = Ray(hd.point, m2w * br.wo);


            Vertex v(hd, cumulative, cumulative_pdf);
            v.dir = br.wi;
            path.push_back(v);
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
        cumulative *= light->Le(hd.point, dir) * fabs(cos_theta(dir)) / pdf;
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
            br.wi = w2m * ray.get_direction() * -1;

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
            cumulative *= std::max(0.0, cos_theta(br.wo)) / pdf;
            cumulative_pdf *= pdf;

            Vertex v(hd, cumulative, cumulative_pdf);
            v.dir = br.wi;
            path.push_back(v);

            ray = Ray(hd.point + br.wo * EPSILON, m2w * br.wo);
            ++niter;
        }
        return path;
    }

}