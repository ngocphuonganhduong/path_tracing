#include "bsdf.hh"

namespace pathtracing {
    /**
     * SAMPLING PHONG MODEL : GLOSSY SPECULAR + DIFFUSE
     *
     * **/
    Vector3 BlinnPhongBSDF::sampleBSDF(BSDFRecord &data, double &pdf) const {
        double rnd = random_uniform();
        if (rnd >= pd + ps)
            return Vector3(0.0);
        Vector3 wh;
        if (rnd < pd) {
            data.wo = cosineSampleHemisphere(pdf); //pdf = cos_theta/PI
            return kd_ * M_1_PI / pd;
        }
        //Halfway vector
        wh = cosinePowerSampleHemisphere(pdf, ns_); //halfway vector around normal
        data.wo = (data.wi * -1).reflect(wh); //wo is the reflected ray of wi respecting to wh

        //pdf /= std::min(2.0, 4.0 * fabs(Vector3::cos(data.wo, wh)));
        pdf /= 4.0 * fabs(Vector3::cos(data.wo, wh));

        //(n + 2) * (n + 4) / (8  * M_PI * pow(2, -n * 0.5) + n)
        //return kd_ * (ns_ + 2) * (ns_ + 4) / (8  * M_PI * pow(2, -ns_ * 0.5) + ns_) *  (pow(cos_theta(wh), ns_)) * ps;

        return ks_ * (ns_ + 2) * 0.5 * M_1_PI * (pow(cos_theta(wh), ns_)) / ps;
    }

    double BlinnPhongBSDF::pdf(const BSDFRecord &data) const {
        double rnd = random_uniform();
        double pdf = 1.0;
        if (rnd < pd) {
            pdf = cosineSampleHemispherePDF(data);
        }
        if (rnd < pd + ps) {
            Vector3 wh = (data.wo + data.wi).normalize();
            pdf = cosinePowerSampleHemispherePDF(data, ns_) / (4.0 * fabs(Vector3::cos(data.wo, wh)));
        }
        return pdf;
    }

    Vector3 BlinnPhongBSDF::f(const BSDFRecord &data) const {
        Vector3 wh = (data.wo + data.wi).normalize();
        return kd_ * M_1_PI * pd + ks_ * (ns_ + 2) * 0.5 * M_1_PI * (pow(wh.z(), ns_)) * ps;
    }
}