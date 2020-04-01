#include "bsdf.hh"

namespace pathtracing {
    /**
     * SAMPLING MIRROR EFFECT
     * **/
    Vector3 MirrorBSDF::f(const BSDFRecord &) const {
        //mirror cannot be brightened by the light
        return Vector3(0.0);
    }

    double MirrorBSDF::pdf(const BSDFRecord &data) const {
        return cos_theta(data.wo);
    }

    Vector3 MirrorBSDF::sampleBSDF(BSDFRecord &data, double &pdf) const {
        data.wo = data.wi.reflect_model_space();
        pdf = 1.0;
        return ks_;
    }
}