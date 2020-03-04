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
        float ka;
        float kd;
        float ks;
        float shininess;
        float reflectivity;
        Vector3 specular;
    };

    using shared_mat = std::shared_ptr<Material>;
}
#endif /* MATERIAL_HH */
