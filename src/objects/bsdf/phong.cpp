#include "bsdf.hh"

namespace pathtracing {
    /**
     * SAMPLING PHONG MODEL : GLOSSY SPECULAR + DIFFUSE
     *
     * **/
    Vector3 PhongBSDF::sampleBRDF(BSDFRecord &data, double &pdf) const {
        double rnd = drand48();
        if (rnd < pd) {
            Vector3 r = data.wi.reflect_model_space();
            data.wo = cosineSampleHemisphere(pdf); //pdf = cos_theta/PI
            pdf *= pd;
            return mat_->kd * M_1_PI * pd; //+ ks_ * (ns_ + 2) * 0.5 * M_1_PI * (pow(fabs(Vector3::cos(data.wo, r)), ns_)) * ps;
        } else if (rnd < pd + ps) {
            data.wo = data.wi.reflect_model_space(); //reflected ray, projected on surface normal space
            Vector3 wo = cosinePowerSampleHemisphere(pdf, mat_->ns); //glossy ray, projected on reflected ray
            Matrix3x3 r2n = Matrix3x3::modelToWorld(data.wo); //transform matrix from reflected space to normal space
            data.wo = r2n * wo;
            pdf *= ps;
            return mat_->ks * (mat_->ns + 2) * 0.5 * M_1_PI * (pow(wo.z(), mat_->ns)) * ps;//kd_ * M_1_PI * pd +
        }

        return Vector3(0);
    }


    double PhongBSDF::pdf(const BSDFRecord &data) const {
        double rnd = random_uniform();
        if (rnd < pd) {
            return cosineSampleHemispherePDF(data);
        }
        if (rnd < pd + ps) {
            return cosinePowerSampleHemispherePDF(data, mat_->ns);
        }
        return 0.0;
    }

    Vector3 PhongBSDF::brdf(const BSDFRecord &data, double& pdf) const {
        double rnd = random_uniform();
        if (rnd < pd) {
            pdf = cosineSampleHemispherePDF(data) / pd;
            return mat_->kd * M_1_PI * pd;
        }
        if (rnd < pd + ps) {
            Vector3 wr = data.wi.reflect_model_space(); //reflected ray, projected on surface normal space
            pdf =  cosinePowerSampleHemispherePDF(data, mat_->ns) / ps;
            return mat_->ks * (mat_->ns + 2) * 0.5 * M_1_PI * (pow(fabs(Vector3::cos(wr, data.wo)), mat_->ns)) * ps;
        }
        pdf = 1 / (1 - pd - ps);
        return  Vector3(0.0);
    }
}