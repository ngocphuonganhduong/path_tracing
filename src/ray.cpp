#include "ray.hh"

namespace raytracing {
    Ray::Ray(const Vector3& origin, const Vector3& direction)
        : origin_(origin), direction_(direction){};
    Vector3 Ray::get_origin() const {
        return this->origin_;
    }
    Vector3 Ray::get_direction() const {
        return this->direction_;
    }
    std::ostream& operator<<(std::ostream& out, Ray const& r) {
        return out << "ray: O" << r.origin_ << " + D" << r.direction_;
    }
}
