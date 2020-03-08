#ifndef MATERIAL_HH
#define MATERIAL_HH
#include <memory>
#include <cmath>
#include "../utils/vector3.hh"

namespace pathtracing {
    class Material {
    public:
        Material(float ka_, float kd_, float ks_, float shininess_,
                 float reflectivity_, const Vector3& specular_)
            : ka(ka_), kd(kd_), ks(ks_), shininess(shininess_),
              reflectivity(reflectivity_), specular(specular_) {}
        Material(float ka_, float kd_, float ks_, float shininess_,
                 float reflectivity_, const Vector3& specular_,
                 float a_, float b_, float c_)
            : ka(ka_), kd(kd_), ks(ks_), shininess(shininess_),
              reflectivity(reflectivity_), specular(specular_),
              a(a_), b(b_), c(c_){}

        Material(float a_, float b_, float c_): a(a_), b(b_), c(c_) {}
        float ka;
        float kd;
        float ks;
        float shininess;
        float reflectivity;
        Vector3 specular;

        //Control the intensity of diffuse = 1.0/f(distance);
        //f(d) = a*distance*distance + b*distance + c
        float a = 0;
        float b = 0;
        float c = 1.0;
    };

    using shared_mat = std::shared_ptr<Material>;
}
#endif /* MATERIAL_HH */
