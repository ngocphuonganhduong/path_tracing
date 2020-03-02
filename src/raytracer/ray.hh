#ifndef RAY_HH
#define RAY_HH

#include "vector3.hh"

namespace raytracing {
    class Ray {
    public:
        Ray(const Vector3& origin, const Vector3& direction);
        Vector3 get_origin() const;
        Vector3 get_direction() const;
        friend std::ostream& operator<<(std::ostream& out, Ray const& r);
    private:
        Vector3 origin_;
        Vector3 direction_;
    };

    struct HitRecord {
        unsigned int object_id;
        Vector3 point;
        Vector3 direction; //from camera to hit point
        Vector3 normal;
        float smooth_ratio;
    };

}
#endif /* RAY_HH */
