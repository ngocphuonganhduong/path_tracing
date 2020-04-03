#include "bsdf.hh"

namespace pathtracing {

    /**
     * DIFFUSE BRDF
     *
     * **/
    Vector3 DiffuseBSDF::f(const BSDFRecord &) const {
        return mat_->kd * M_1_PI;
    }

    double DiffuseBSDF::pdf(const BSDFRecord &data) const {
        return cos_theta(data.wo) * M_1_PI; //cos_theta / PI
    }

    Vector3 DiffuseBSDF::sampleBSDF(BSDFRecord &data, double &pdf) const {
        data.wo = cosineSampleHemisphere(pdf); //pdf = cos_theta/PI
        return mat_->kd * M_1_PI; //brdf of diffuse is kd_ / pi
    }

}