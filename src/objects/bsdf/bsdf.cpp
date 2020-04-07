//
// Created by rimuru on 03/04/2020.
//
#include "bsdf.hh"

namespace pathtracing {

    double BSDF::computeFresnelReflectivity(double cosI, const double &n1, const double &n2) {
        double R0 = (n1 - n2) / (n1 + n2);
        R0 *= R0;

        const double n = n1 / n2;
        if (n1 > n2) {
            double sinT2 = n * n * (1 - cosI * cosI);
            if (sinT2 > 1.0)
                return 1.0;//total internal reflection
            cosI = sqrt(1.0 - sinT2);
        }
        const double f = 1.0 - cosI;
        return R0 + (1.0 - R0) * f * f * f * f * f; //r0 + (1.0 - r0) * f^5

    }

    Vector3 BSDF::evalSampleBSDF(BSDFRecord &data, double &pdf) const {
        double rnd = drand48();
        Vector3 f;
        if (rnd < mat_->d) //bsdf
        {
            f = sampleBRDF(data, pdf) * mat_->d;
            pdf *= mat_->d;
            f *= fabs(cos_theta(data.wo)) / pdf; //wo here is direction from light to hit point.

//            if (!data.wi.refract(data.wo, 1.0 / mat_->ni)) {
//                f = sampleBRDF(data, pdf) * mat_->d;
//                pdf *= mat_->d;
//                f *= fabs(cos_theta(data.wo)) / pdf; //wo here is direction from light to hit point.
//
//            } else {
//                double kr = computeFresnelReflectivity(cos_theta(data.wi), 1, mat_->ni);
//                double rnd = drand48();
//
//                if (rnd < kr) {
//                    f = sampleBRDF(data, pdf) * mat_->d;
//                    pdf *= mat_->d;
//                    f *= fabs(cos_theta(data.wo)) / pdf; //wo here is direction from light to hit point.
//                } else {
//                    f = Vector3(0);
//                    pdf = 1.0;
//                }
//            }

        } else {
            f = mat_->tf; // * 1 - mat_->d / pdf = 1.0
            data.wo = data.wi * -1;
            pdf = 1 - mat_->d;
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