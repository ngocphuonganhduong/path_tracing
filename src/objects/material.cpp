//
// Created by rimuru on 28/03/2020.
//

#include "material.hh"
#include "../utils/sampler.hh"

namespace pathtracing {
    Material::Material(const Vector3 &ka_, const Vector3 &kd_, const Vector3 &ks_, double ns_,
                       double ni_, double d_) : ka(ka_), kd(kd_), ks(ks_), ns(ns_), ni(ni_),
                                                d(d_) {
    }

    Material::Material(const Vector3 &ka_, const Vector3 &kd_, const Vector3 &ks_, double ns_) : ka(ka_), kd(kd_),
                                                                                                 ks(ks_), ns(ns_) {}

    void Material::set_emission(const Vector3 &ke_, double a_, double b_, double c_) {
        ke = ke_;
        a = a_;
        b = b_;
        c = c_;
    }
}