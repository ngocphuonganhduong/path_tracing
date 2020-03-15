#ifndef MATERIAL_HH
#define MATERIAL_HH
#include <memory>
#include <cmath>
#include "../utils/vector3.hh"

namespace pathtracing {
    class Material {
    public:
        Material(const Vector3& ka_, const Vector3& kd_, const Vector3& ks_,
                 float ns_, float kr_, float ksm_)
            : ka(ka_), kd(kd_), ks(ks_), ns(ns_), kr(kr_), ksm(ksm_) {}

        Material(const Vector3& ka_, const Vector3& kd_, const Vector3& ks_,
                 float ns_, float kr_, float ksm_, float a_, float b_, float c_)
            : ka(ka_), kd(kd_), ks(ks_), ns(ns_), kr(kr_), ksm(ksm_),
              a(a_), b(b_), c(c_){}

        Material(float a_, float b_, float c_): a(a_), b(b_), c(c_) {}
        Vector3 ka;
        Vector3 kd;
        Vector3 ks;
        float ns; //shininess
        float kr; //reflectivity coef
        float ksm; //smoothness [0, 1] from blur to clear effect

        //Control the intensity of diffuse = 1.0/f(distance);
        //f(d) = a*distance*distance + b*distance + c
        float a = 0;
        float b = 0;
        float c = 1.0;
    };

    using shared_mat = std::shared_ptr<Material>;
}
#endif /* MATERIAL_HH */
