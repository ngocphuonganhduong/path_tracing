#include "bsdf.hh"

namespace pathtracing {
    /**
     * SAMPLING PHONG MODEL : GLOSSY SPECULAR + DIFFUSE
     *
     * **/
    Vector3 PhongBSDF::sampleBSDF(BSDFRecord &data, double &pdf) const {
        double rnd = random_uniform();
        if (rnd < pd) {
            Vector3 r = data.wi.reflect_model_space();
            data.wo = cosineSampleHemisphere(pdf); //pdf = cos_theta/PI
            return kd_ * M_1_PI * pd + ks_ * (ns_ + 2) * 0.5 * M_1_PI * (pow(fabs(Vector3::cos(data.wo, r)), ns_)) * ps;
        } else if (rnd < pd + ps) {
            data.wo = data.wi.reflect_model_space(); //reflected ray, projected on surface normal space
            Vector3 wo = cosinePowerSampleHemisphere(pdf, ns_); //glossy ray, projected on reflected ray
            Matrix3x3 r2n = Matrix3x3::modelToWorld(data.wo); //transform matrix from reflected space to normal space
            data.wo = r2n * wo;
            return kd_ * M_1_PI * pd + ks_ * (ns_ + 2) * 0.5 * M_1_PI * (pow(wo.z(), ns_)) * ps;
        }

        return Vector3(0);
    }


    double PhongBSDF::pdf(const BSDFRecord &data) const {
        double rnd = random_uniform();
        if (rnd < pd) {
            return cosineSampleHemispherePDF(data);
        }
        if (rnd < pd + ps) {
            return cosinePowerSampleHemispherePDF(data, ns_);
        }
        return 0.0;
    }

    Vector3 PhongBSDF::f(const BSDFRecord &data) const {
        Vector3 wr = data.wi.reflect_model_space(); //reflected ray, projected on surface normal space
        return kd_ * M_1_PI * pd + ks_ * (ns_ + 2) * 0.5 * M_1_PI * (pow(fabs(Vector3::cos(wr, data.wo)), ns_)) * ps;
    }
}