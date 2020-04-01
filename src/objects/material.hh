#ifndef MATERIAL_HH
#define MATERIAL_HH

#include <memory>
#include <cmath>
#include "../utils/vector3.hh"
#include "bsdf.hh"

namespace pathtracing {
    class Material {
    public:
        enum BSDFType {
            DIFFUSE = 0, MIRROR, PHONG, BLINN_PHONG
        };

        static std::shared_ptr<Material> create_mat(BSDFType type, const Vector3 &ka_, const Vector3 &kd_, const Vector3 &ks_, double ns_);
        static std::shared_ptr<Material>  create_mat(const Vector3 &ka_, const Vector3 &kd_, const Vector3 &ks_, double ns_);
        static std::shared_ptr<Material>  create_diffuse_mat(const Vector3 &ka_, const Vector3 &kd_);
        static std::shared_ptr<Material>  create_mirror_mat(const Vector3 &ka_, const Vector3 &ks_);
        static std::shared_ptr<Material>  create_phong_mat(const Vector3 &ka,  const Vector3 &kd, const Vector3 &ks, const double& ns);

        Material(BSDFType type, const Vector3 &ka_, const Vector3 &kd_, const Vector3 &ks_, double ns_, double sharpness, double ni_,
                 double d_);

//        Material(const Vector3 &ka_, const Vector3 &kd_, const Vector3 &ks_, double ns_, double ni_,
//                 double d_);
//        Material(const Vector3 &ka_, const Vector3 &kd_, const Vector3 &ks_, double ns_) : ka(ka_), kd(kd_), ks(ks_), ns(ns_){}

        void set_emission(const Vector3 &ke_, double a_, double b_, double c_) {
            ke = ke_;
            a = a_;
            b = b_;
            c = c_;
        }

//        BSDFType type = DIFFUSE;
        Vector3 ka;
        Vector3 kd;
        Vector3 ks;
        double ns = 1; //shininess
        double ni = 1; // optical density (aka index of refraction
        double d = 1; //opacity
        double sharpness = 60; //ranging from 0 to 1000

        //Control the intensity of diffuse = 1.0/f(distance);
        //f(d) = a*distance*distance + b*distance + c
        Vector3 ke;
        double a = 0;
        double b = 0.5;
        double c = 1.0;
        shared_bsdf bsdf;
    };

    using shared_mat = std::shared_ptr<Material>;
}
#endif /* MATERIAL_HH */
