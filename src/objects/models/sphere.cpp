#include "model.hh"

namespace pathtracing {
    Sphere::Sphere(float radius_) : radius(radius_) {}

    Vector3 Sphere::get_sample(const Vector3& pos) const {
        double a = 2 * M_PI * drand48();
        double b = 2 * M_PI * drand48();
        Vector3 s(cos(a) * cos(b), cos(a) * sin(b), sin(a));
        return pos + s * radius;
    }

    bool Sphere::hit (const Vector3& pos, const Ray& r,
                      HitRecord& hit_data) const
    {
        Vector3 l = r.get_origin() - pos;
        //( t*t* u*u + 2*t*u*l + l*l - r*r)
        float a = r.get_direction().dot(r.get_direction());
        float b = 2 * l.dot( r.get_direction());

        float discriminant = (b * b) - (a * (l.dot(l) -
                                             this->radius *
                                             this->radius) * 4);

        if (discriminant < 0) {
            return false;
        }
        float t = (-b - sqrt(discriminant) ) / (2.0 * a);
        if (t < 0) {
            return false;
        }
        hit_data.point = r.get_origin() + r.get_direction() * t;
        hit_data.normal = hit_data.point - pos;
        hit_data.normal.normalize();
        return true;
    }
}
