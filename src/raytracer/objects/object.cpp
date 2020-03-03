#include "object.hh"

namespace raytracing {
    Object::Object(shared_texture texture, Vector3 position)
        : texture(texture), position(position){}

    shared_texture Object::get_texture() const {
        return this->texture;
    }
}
