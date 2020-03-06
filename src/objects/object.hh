#ifndef OBJECT_HH
#define OBJECT_HH
#include "material.hh"
#include "texture.hh"
#include "models/model.hh"


namespace pathtracing {
    class Object {
    public:
        Object(const Vector3& position, const Vector3& emitted_rad,
               shared_mat mat, shared_text text, shared_mod model);
        Object(const Vector3& position, shared_mat mat, shared_text text,
               shared_mod model);
        bool hit(const Ray&, HitRecord& hit_data) const;
        Vector3 get_sample() const;
        Vector3 position; //world position
        Vector3 emitted_rad;
        shared_mat material;
        shared_text texture;
        shared_mod model;
    };
    using shared_obj = std::shared_ptr<Object>;
}

#endif /* OBJECT_HH */
