//
// Created by rimuru on 26/03/2020.
//

#ifndef sampler_HH
#define sampler_HH

#include "vector3.hh"

namespace pathtracing {
    struct BSDFRecord {
        Vector3 wi; //model/obj space
        Vector3 wo; //model/obj space

    };

    constexpr inline double cos_theta(const Vector3 &v) {
        return v.z();
    }


    Vector3 cosineSampleHemisphere(double &pdf);
    double cosineSampleHemispherePDF(const BSDFRecord &data);


    Vector3 cosinePowerSampleHemisphere(double &pdf, const double &n);
    double cosinePowerSampleHemispherePDF(const BSDFRecord &data, const double &n);


    Vector3 uniformSampleSphere(double &pdf);
    double uniformSampleSpherePDF();

//    Vector3 sample_glossy(double &pdf, const double &n, const double &limit_angle);

    Vector3 sampleHemisphere(double &pdf, const Vector3 &normal);
    inline double random_uniform() {
        return ((double) std::rand()) / RAND_MAX;
    }
}

#endif //sampler_HH
