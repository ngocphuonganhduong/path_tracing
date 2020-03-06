#include "object.hh"

namespace pathtracing {

    Object::Object(const Vector3& pos_, const Vector3& e_rad_,
           shared_mat mat_, shared_text text_, shared_mod model_)
        : position(pos_), emitted_rad(e_rad_), material(mat_),
          texture(text_), model(model_) {}

    Object::Object(const Vector3& pos_, shared_mat mat_, shared_text text_,
                   shared_mod model_)
        : position(pos_), material(mat_), texture(text_), model(model_) {}

    bool Object::hit(const Ray& r, HitRecord& hit_data) const
    {
        return model->hit(position, r, hit_data);
    }

    Vector3 Object::get_sample() const {
        return model->get_sample(position);
    }

}
