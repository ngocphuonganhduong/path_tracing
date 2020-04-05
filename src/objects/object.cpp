#include "../render.hh"
#include "../utils/sampler.hh"
#include "object.hh"


namespace pathtracing {


    Object::Object(const Vector3 &pos_, shared_bsdf bsdf_)
            : position(pos_), bsdf(bsdf_) {}

    Vector3 Object::Le(const Vector3 &pos, const Vector3 &direction) const {
        return bsdf->ke();
    }


//    Ray Object::sampleLightRay(double &pdfForward, double &pdfBackward) const {
//        Vector3 normal;
//        Vector3 pos = sampleSurfacePosition(pdfBackward);
//        return Ray(pos, sampleHemisphere(pdfForward, normal));
//    }

//
//    Vector3 Object::phong_lighting(const Vector3 &li, const Vector3 &wi, const Vector3 &wo, const Vector3 &normal,
//                                   const double &d2) {
//        Vector3 dir = wi * -1;
//        float ld = normal.dot(dir);
//        Vector3 color;
//        if (ld > 0) {
//            //DIFFUSE LIGHT
//            color += material->kd * ld / d2;
//
//            Vector3 r = dir.reflect(normal); //reflected vector
//            float ls = wo.dot(r) / (wo.norm() * r.norm());
//
//            if (ls > 0)
//                color += material->ks * pow(ls, material->ns);
//        }
//        return color;
//    }

}
