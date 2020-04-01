#include "bsdf.hh"
#include "../utils/sampler.hh"
#include "../utils/matrix.hh"

namespace pathtracing {

    /**
     * DIFFUSE BRDF
     *
     * **/

    Vector3 DiffuseBSDF::f(const BSDFRecord &) const {
        return kd_ * M_1_PI;
    }

    double DiffuseBSDF::pdf(const BSDFRecord &data) const {
        return cos_theta(data.wo) * M_1_PI; //cos_theta / PI
    }

    Vector3 DiffuseBSDF::sampleBSDF(BSDFRecord &data, double &pdf) const {
        data.wo = cosineSampleHemisphere(pdf); //pdf = cos_theta/PI
        return kd_ * M_1_PI; //brdf of diffuse is kd_ / pi
    }

    /**
      * PERFECT SPECULAR BRDF
      *
      * **/

    Vector3 MirrorBSDF::f(const BSDFRecord &) const {
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

    /**
   * BRDF SAMPLING PHONG MODEL : GLOSSY SPECULAR + DIFFUSE
   *
   * **/
    Vector3 PhongBSDF::sampleBSDF(BSDFRecord &data, double &pdf) const {
        double rnd = random_uniform();
        if (rnd < pd) {
            data.wo = cosineSampleHemisphere(pdf); //pdf = cos_theta/PI
            return kd_ * M_1_PI;
        } else if (rnd < pd + ps) {
            data.wo = data.wi.reflect_model_space(); //reflected ray, projected on surface normal space
            Vector3 wo = cosinePowerSampleHemisphere(pdf, ns_); //glossy ray, projected on reflected ray
            Matrix3x3 r2n = Matrix3x3::modelToWorld(data.wo); //transform matrix from reflected space to normal space
            data.wo = r2n * wo;
            return ks_ * (ns_ + 2) * 0.5 * M_1_PI * (pow(wo.z(), ns_));
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
        Vector3 f;
        if (pd > 0) {
            f += kd_ * M_1_PI * pd;
        }
        if (ps > 0) {
            Vector3 wr = data.wi.reflect_model_space(); //reflected ray, projected on surface normal space
            f += ks_ * (ns_ + 2) * 0.5 * M_1_PI * (pow(fabs(Vector3::cos(wr, data.wo)), ns_)) * ps;
        }
        return f;
    }


}