#include "object.hh"
#include <math.h>

namespace raytracing {
    // Object
    Object::Object(shared_texture texture, Vector3 position)
        : texture(texture), position(position){}

    shared_texture Object::get_texture() const {
        return this->texture;
    }
    ///SPHERE
    Sphere::Sphere(shared_texture texture,
                   const Vector3& center, float r) :
        Object(texture, center), radius(r){}
/*
    float Sphere::get_smooth_step(const Ray& r) const{
        Vector3 oc = r.get_origin() - this->position;
        float cos = oc.dot(r.get_direction()) / (oc.norm() * r.get_direction().norm());
//        std::cout << cos << "\n";
        float t = oc.norm() * sqrt(1 - cos*cos);
        //      std::cout << this->radius << " / " << t << " \n";
        return this->radius / t;
    }
*/
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

    //Plane
    /*
    Plane::Plane(shared_texture texture, const Vector3& point,
                 const Vector3& normal)
        : Object(texture, point), normal(normal){}

    bool Plane::hit (const Ray& r, HitRecord& hit_data) const {
        float denom = this->normal.dot(r.get_direction());
        if (std::abs(denom) > 0.0001) { //not pendicular to normal
            float t = (this->position - r.get_origin()).dot(this->normal) / denom;
            if (t > 0)
            {
                hit_data.point = r.get_origin() + r.get_direction() * t;
                hit_data.direction = hit_data.point - r.get_origin();
                hit_data.normal = this->normal;
                hit_data.normal.normalize();
                return true;
            }
        }
        return false;
        }*/

    //Square
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
            //float t = -(this->normal.dot(hit_data.point) + this->position).dot(this->normal)
            float t = (this->position - r.get_origin()).dot(this->normal) / denom;
//            std::cout << "t : " << t << "\n";
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
