#include "../object.hh"
#include "../../utils/sampler.hh"

namespace pathtracing {
    Square::Square(const Vector3 &position, shared_bsdf bsdf, const Vector3 &normal, const Vector3 &up, float halfSize_)
            : Object(position, bsdf), halfSize(halfSize_) {
        this->normal = normal;
        this->up = up;
        this->right = up.cross(normal);
        this->up.normalize();
        this->normal.normalize();
    }

    double Square::sampleSurfacePositionPDF(HitRecord &hit_data) const {
        return 1.0 / (halfSize * halfSize);
    }

    double Square::sampleDirectionPDF(const BSDFRecord &data) const {
        return cosineSampleHemispherePDF(data);
    }
//    Vector3 Square::get_sample() const {
//        return position + up * (-1 + 2 * drand48()) * halfSize
//               + right * (-1 + 2 * drand48()) * halfSize;
//    }

    Vector3 Square::sampleSurfacePosition(double &pdf, Vector3 &surfaceNormal) const {
        pdf = 1.0 / (halfSize * halfSize);
        surfaceNormal = normal;
        return position + up * (-1 + 2 * drand48()) * halfSize + right * (-1 + 2 * drand48()) * halfSize;
    }


    bool Square::hit(const Ray &r, HitRecord &hit_data) const {
        float denom = this->normal.dot(r.get_direction());
        if (std::abs(denom) > 0.000001) {
            float t = (position - r.get_origin()).dot(this->normal) / denom;
            if (t > 0.00001) {
                hit_data.point = r.get_origin() + r.get_direction() * t;
                hit_data.normal = this->normal;
                hit_data.normal.normalize();

                Vector3 point_to_center = position - hit_data.point;
                float norm = point_to_center.norm();
                float cos_alpha = point_to_center.dot(this->up) / (norm);
                if (std::abs(cos_alpha) * norm <= halfSize) {
                    cos_alpha = point_to_center.dot(this->right) / (norm);
                    if (std::abs(cos_alpha) * norm <= halfSize) {
                        return true;
                    }
                }
            }
        }
        return false;
    }

    Vector3 Square::Le(const Vector3 &, const Vector3 &wo) const {
        if (cos_theta(wo) <= 0)
            return Vector3(0.0);
        return bsdf->ke();
    }
}
