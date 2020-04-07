#include "bsdf.hh"

namespace pathtracing {
    /**
     * SAMPLING MIRROR EFFECT
     * **/
    Vector3 MirrorBSDF::brdf(const BSDFRecord &data, double& pdf) const {
        pdf = cos_theta(data.wo);
        //mirror cannot be brightened by the light
        return Vector3(0.0);
    }

    double MirrorBSDF::pdf(const BSDFRecord &data) const {
        return cos_theta(data.wo);
    }

    Vector3 MirrorBSDF::sampleBRDF(BSDFRecord &data, double &pdf) const {
        data.wo = data.wi.reflect_model_space();
        pdf = this->pdf(data);
        return mat_->ks;
    }
}