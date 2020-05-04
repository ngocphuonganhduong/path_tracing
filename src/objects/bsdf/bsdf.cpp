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
        Vector3 f(1.0);
        if (rnd < mat_->d) //bsdf
        {
            if (!data.wi.refract(data.wo, 1.0 / mat_->ni)) {
                f = sampleBRDF(data, pdf);
                f *= fabs(cos_theta(data.wo)) / pdf; //wo here is direction from light to hit point.

            } else {
                double kr = computeFresnelReflectivity(cos_theta(data.wi), 1, mat_->ni);
                double rnd = drand48();

                if (rnd < kr) {
                    f = sampleBRDF(data, pdf);
                    f *= fabs(cos_theta(data.wo)) / pdf; //wo here is direction from light to hit point.
                    pdf *= kr;
                } else {
                    pdf = (1 - kr);
                }
            }
            pdf *= mat_->d;

        }
        else {
            f = mat_->tf; // * 1 - mat_->d / pdf = 1.0
            data.wo = data.wi * -1;
            pdf = 1 - mat_->d;
        }
        return f;
    }

    Vector3 BSDF::evalBSDF(const BSDFRecord &data, double &pdf) const {
        double n = 1.0 / mat_->ni;

        if (data.wo.z() <= 0)
            n = 1.0 / n;
        const double sinT2 = n * n * std::max(0.0, (1 - data.wo.z() * data.wo.z()));

        if (sinT2 >= 1) {
            return brdf(data, pdf) * mat_->d;
        }
        double kr = computeFresnelReflectivity(cos_theta(data.wo), 1, mat_->ni);
        double rnd = drand48();
        Vector3 f;
        if (rnd < kr) {
            f = brdf(data, pdf) * mat_->d;
            pdf *= kr;
        } else {
            pdf = (1 - kr);
        }
        return f;
    }

}