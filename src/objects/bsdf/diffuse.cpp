#include "bsdf.hh"

namespace pathtracing {

    /**
     * DIFFUSE BRDF
     *
     * **/
    Vector3 DiffuseBSDF::brdf(const BSDFRecord &data,  double& pdf) const {
        pdf = cosineSampleHemispherePDF(data);
        return mat_->kd * M_1_PI;
    }

    double DiffuseBSDF::pdf(const BSDFRecord &data) const {
        return cosineSampleHemispherePDF(data); //cos_theta / PI
    }

    Vector3 DiffuseBSDF::sampleBRDF(BSDFRecord &data, double &pdf) const {
        data.wo = cosineSampleHemisphere(pdf); //pdf = cos_theta/PI
        return mat_->kd * M_1_PI; //brdf of diffuse is kd_ / pi
    }


}