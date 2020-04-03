#include "../object.hh"
#include "../../utils/sampler.hh"

namespace pathtracing {
    Sphere::Sphere(const Vector3 &position, shared_bsdf bsdf, float radius_)
            : Object(position, bsdf), radius(radius_) {}
//
//    Sphere::Sphere(const Vector3 &position, shared_mat mat, const Vector3 &color, float radius_)
//            : Object(position, mat, color), radius(radius_) {}
//

    Vector3 Sphere::sampleSurfacePosition(double &pdf) const {
        Vector3 surfaceNormal = uniformSampleSphere(pdf);
        return position +  surfaceNormal * radius;
    }
    double Sphere::sampleSurfacePositionPDF() const {
        return uniformSampleSpherePDF();
    }

    double  Sphere::sampleDirectionPDF(const BSDFRecord&) const {
        return uniformSampleSpherePDF();
    }

    bool Sphere::hit(const Ray &r, HitRecord &hit_data) const {
        Vector3 l = r.get_origin() - position;
        //( t*t* u*u + 2*t*u*l + l*l - r*r)
        float a = r.get_direction().dot(r.get_direction());
        float b = 2 * l.dot(r.get_direction());

        float discriminant = (b * b) - (a * (l.dot(l) -
                                             this->radius *
                                             this->radius) * 4);

        if (discriminant < 0) {
            return false;
        }
        float t = (-b - sqrt(discriminant)) / (2.0 * a);
        if (t < 0) {
            return false;
        }
        hit_data.point = r.get_origin() + r.get_direction() * t;
        hit_data.normal = hit_data.point - position;
        hit_data.normal.normalize();
        return true;
    }
}
