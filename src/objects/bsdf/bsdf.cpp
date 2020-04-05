//
// Created by rimuru on 03/04/2020.
//
#include "bsdf.hh"

namespace pathtracing {

    double BSDF::computeFresnelReflectivity(double cosI, const double &n1, const double &n2) {
        const double sinT = (n1 / n2) * sqrt(std::max(0.0, 1 - cosI * cosI));
        if (sinT > 1)
            return 1.0; //total internal reflection
        double cosT = sqrt(std::max(0.0, 1 - sinT * sinT));
        cosI = fabs(cosI);

        float Rs = ((n2 * cosI) - (n1 * cosT)) / ((n2 * cosI) + (n1 * cosT));
        float Rp = ((n1 * cosI) - (n2 * cosT)) / ((n1 * cosI) + (n2 * cosT));
        return (Rs * Rs + Rp * Rp) * 0.5;
    }

    Vector3 BSDF::evalSampleBSDF(BSDFRecord &data, double &pdf) const {
        double rnd = drand48();
        Vector3 f;
        if (rnd < mat_->d) //bsdf
        {
            f = sampleBRDF(data, pdf);
            f *= fabs(cos_theta(data.wo)) / pdf; //wo here is direction from light to hit point.
            pdf *= mat_->d;
        } else {
            f = mat_->tf;
            data.wo = data.wi * -1;
            pdf *= 1 - mat_->d;
        }
        return f;

//        double kr = computeFresnelReflectivity(cos_theta(data.wi), 1, mat_->ni);
//        std::cout << kr << "\n";
//        double rnd = drand48();
//        Vector3 f = sampleBRDF(data, pdf);
//        if (rnd < kr) //internal reflection
//        {
//            return f * kr;
//        }
//        //total refraction
//        data.wo.flip_z();
//        return (1 - kr);
    }

    Vector3 BSDF::f(const BSDFRecord &data, double &pdf) const {
        return brdf(data, pdf) * mat_->d;
    }

//    Vector3 BSDF::evalBSDF(BSDFRecord &data, double &pdf) const {
//        return evalBRDF(data) * mat_->d;
//    }
}