#ifndef RAY_HH
#define RAY_HH

#include "vector3.hh"

namespace pathtracing {
    class Ray {
    public:
        constexpr Ray() noexcept = default;

        constexpr Ray(const Vector3 &origin, const Vector3 &direction) noexcept  : origin_(origin),
                                                                                   direction_(direction) {};

        constexpr Vector3 get_origin() const noexcept {
            return origin_;
        }

        constexpr Vector3 get_direction() const noexcept {
            return direction_;
        }

        friend std::ostream &operator<<(std::ostream &out, Ray const &r) {
            return out << "ray: O" << r.origin_ << " + D" << r.direction_;
        }

    private:
        Vector3 origin_{};
        Vector3 direction_{};
    };


}
#endif /* RAY_HH */
