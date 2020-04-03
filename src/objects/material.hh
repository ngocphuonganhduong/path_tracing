#ifndef MATERIAL_HH
#define MATERIAL_HH

#include <memory>
#include <cmath>
#include "../utils/vector3.hh"
//#include "bsdf/bsdf.hh"

namespace pathtracing {
    class Material {
    public:
        Material(const Vector3 &ka_, const Vector3 &kd_, const Vector3 &ks_, double ns_);

        Material(const Vector3 &ka_, const Vector3 &kd_, const Vector3 &ks_, double ns_, double ni_,
                 double d_);

        void set_emission(const Vector3 &ke_, double a_, double b_, double c_);

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
    };

    using shared_mat = std::shared_ptr<Material>;
}
#endif /* MATERIAL_HH */
