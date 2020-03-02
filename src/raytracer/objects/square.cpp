#include "object.hh"
#include <cmath>

namespace raytracing {
    Square::Square(shared_texture texture, const Vector3& center,
                   const Vector3& normal, const Vector3& up, float distance)
        : Object(texture, center), distance(distance)
    {
        this->normal = normal;
        this->up = up;
        this->right = up.cross_product(normal);
        this->up.normalize();
        std::cout << right ;
        this->normal.normalize();
    }

    bool Square::hit (const Ray& r, HitRecord& hit_data) const {
        float denom = this->normal.dot(r.get_direction());
        if (std::abs(denom) > 0.000001) {
            float t = (this->position - r.get_origin()).dot(this->normal) / denom;
            if (t > 0.00001)
            {
                hit_data.point = r.get_origin() + r.get_direction() * t;
                hit_data.direction = hit_data.point - r.get_origin();
                hit_data.normal = this->normal;
                hit_data.normal.normalize();

                Vector3 point_to_center = this->position - hit_data.point;
                float norm = point_to_center.norm();
                float cos_alpha = point_to_center.dot(this->up)/(norm);
                if (std::abs(cos_alpha) * norm <= distance) {
                    cos_alpha = point_to_center.dot(this->right)/(norm);
                    if (std::abs(cos_alpha) * norm <= distance) {
                        return true;
                    }
                }
            }
        }
        return false;
    }
}
