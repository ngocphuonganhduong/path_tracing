#include "object.hh"
#include <cmath>


namespace raytracing {
    Sphere::Sphere(shared_texture texture,
                   const Vector3& center, float r) :
        Object(texture, center), radius(r){}


    bool Sphere::hit (const Ray& r, HitRecord& hit_data) const{
        Vector3 l = r.get_origin() - this->position;
        //( t*t* u*u + 2*t*u*l + l*l - r*r)
        float a = r.get_direction().dot(r.get_direction());
        float b = 2 * l.dot( r.get_direction());
        float c = l.dot(l) - this->radius * this->radius;

        float discriminant = (b * b) - (a  * c * 4);

        if (discriminant < 0) {
            return false;
        }
        float t = (-b - sqrt(discriminant) ) / (2.0 * a);

        if (t < 0) {
            return false;
        }
        hit_data.point = r.get_origin() + r.get_direction() * t;
        hit_data.direction = hit_data.point - r.get_origin();
        hit_data.normal = hit_data.point - this->position;
        hit_data.normal.normalize();
        return true;
    }
}
