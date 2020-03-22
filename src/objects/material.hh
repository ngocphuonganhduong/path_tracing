#ifndef MATERIAL_HH
#define MATERIAL_HH
#include <memory>
#include <cmath>
#include "../utils/vector3.hh"

namespace pathtracing {
    class Material {
    public:
        enum Type {DIFFUSE = 0, SPECULAR, GLOSSY};
        Material(Type type_, double ka_, double kd_, const Vector3& ks_, double kr_, double ns_, double ni_, double d_)
            : type(type_), ka(ka_), kd(kd_), ks(ks_), kr(kr_), ns(ns_), ni(ni_), d(d_) {}

        Material(const Vector3& ke_, double a_, double b_, double c_): ke(ke_), a(a_), b(b_), c(c_) {}

        void set_emission(const Vector3& ke_, double a_, double b_, double c_) {
            ke = ke_;
            a = a_;
            b = b_;
            c = c_;
        }

        Type type = DIFFUSE;
        double ka = 1;
        double kd = 1;
        Vector3 ks;
        double kr = 0;
        double ns = 1; //shininess
        double ni = 1; // optical density (aka index of refraction
        double d = 1; //opacity
        //Control the intensity of diffuse = 1.0/f(distance);
        //f(d) = a*distance*distance + b*distance + c
        Vector3 ke;
        double a = 0;
        double b = 0.5;
        double c = 1.0;
    };

    using shared_mat = std::shared_ptr<Material>;
}
#endif /* MATERIAL_HH */
