//
// Created by rimuru on 28/03/2020.
//

#include "material.hh"
#include "../utils/sampler.hh"

namespace pathtracing {
    Material::Material(BSDFType type, const Vector3 &ka_, const Vector3 &kd_, const Vector3 &ks_, double ns_, double sharpness_,
                       double ni_, double d_) : ka(ka_), kd(kd_), ks(ks_), sharpness(sharpness), ns(ns_), ni(ni_), d(d_) {
        switch (type) {
            case DIFFUSE:
                this->bsdf = std::make_shared<DiffuseBSDF>(kd_);
                break;
            case MIRROR:
                this->bsdf = std::make_shared<MirrorBSDF>(ks_);
                break;
            case PHONG:
                this->bsdf = std::make_shared<PhongBSDF>(kd_, ks_, ns_);
                break;
            case BLINN_PHONG:
                this->bsdf = std::make_shared<DiffuseBSDF>(kd_);
                break;
        }
    }

    shared_mat Material::create_mat(Material::BSDFType type, const Vector3 &ka, const Vector3 &kd, const Vector3 &ks,
                                    double ns) {

        return std::make_shared<Material>(type, ka, kd, ks, ns, 1, 1, 1);
    }

    shared_mat Material::create_mat(const Vector3 &ka, const Vector3 &kd, const Vector3 &ks, double ns) {
        return std::make_shared<Material>(DIFFUSE, ka, kd, ks, ns, 1, 1, 1);
    }

    shared_mat Material::create_diffuse_mat(const Vector3 &ka, const Vector3 &kd) {
        return std::make_shared<Material>(DIFFUSE, ka, kd, Vector3(), 1, 1, 1, 1);
    }

    std::shared_ptr<Material> Material::create_mirror_mat(const Vector3 &ka, const Vector3 &ks) {
        return std::make_shared<Material>(MIRROR, ka, Vector3(), ks, 1, 1, 1, 1);
    }

    shared_mat Material::create_phong_mat(const Vector3 &ka, const Vector3 &kd, const Vector3 &ks, const double &ns) {
        return std::make_shared<Material>(PHONG, ka, kd, ks, ns, 1, 1, 1);
    }


}