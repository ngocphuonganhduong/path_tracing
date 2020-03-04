#include "object.hh"

namespace pathtracing {

    Object::Object(const Vector3& pos_, const Vector3& e_light_,
           shared_mat mat_, shared_text text_, shared_mod model_)
        : position(pos_), emitted_light(e_light_), material(mat_),
          texture(text_), model(model_) {}

    bool Object::hit(const Ray& r, HitRecord& hit_data) const
    {
        return model->hit(position, r, hit_data);
    }
}
