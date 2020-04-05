#include "bsdf.hh"

namespace pathtracing {
    /**
     * SAMPLING PHONG MODEL : GLOSSY SPECULAR + DIFFUSE
     *
     * **/
    Vector3 BlinnPhongBSDF::sampleBRDF(BSDFRecord &data, double &pdf) const {
        double rnd = random_uniform();
        if (rnd >= pd + ps)
        {
            pdf = 1.0;
            return Vector3(0.0);
        }
        Vector3 wh;
        if (rnd < pd) {
            data.wo = cosineSampleHemisphere(pdf); //pdf = cos_theta/PI
            pdf *= pd;
            return mat_->kd * M_1_PI * pd;
        }

        //Halfway vector
        wh = cosinePowerSampleHemisphere(pdf, mat_->ns); //halfway vector around normal
        data.wo = (data.wi * -1).reflect(wh); //wo is the reflected ray of wi respecting to wh

        pdf /= std::min(2.0, 4.0 * fabs(Vector3::cos(data.wo, wh)));
//        pdf /= 4.0 * fabs(Vector3::cos(data.wo, wh));
        pdf *= ps;

        return mat_->ks * (mat_->ns + 2) * 0.5 * M_1_PI * (pow(cos_theta(wh), mat_->ns)) * ps;
    }

    double BlinnPhongBSDF::pdf(const BSDFRecord &data) const {
        double rnd = random_uniform();
        double pdf = 1.0;
        if (rnd < pd) {
            pdf = cosineSampleHemispherePDF(data);
        }
        if (rnd < pd + ps) {
            Vector3 wh = (data.wo + data.wi).normalize();
            pdf = cosinePowerSampleHemispherePDF(data, mat_->ns) / (4.0 * fabs(Vector3::cos(data.wo, wh)));
        }
        return pdf;
    }

    Vector3 BlinnPhongBSDF::brdf(const BSDFRecord &data, double& pdf) const {
        double rnd = random_uniform();
        if (rnd < pd){
            pdf = cosineSampleHemispherePDF(data) / pd;
            return mat_->kd * M_1_PI * pd;
        }
        else if (rnd < pd + ps) {
            Vector3 wh = (data.wo + data.wi).normalize();
            pdf = cosinePowerSampleHemispherePDF(data, mat_->ns)  / (4.0 * fabs(Vector3::cos(data.wo, wh)) * ps);
            return mat_->ks * (mat_->ns + 2) * 0.5 * M_1_PI * (pow(wh.z(), mat_->ns)) * ps;
        }
        pdf = 1/(1 - pd - ps);
        return Vector3(0.0);
    }

}