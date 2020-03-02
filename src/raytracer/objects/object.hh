#ifndef OBJECT_HH
#define OBJECT_HH
#include "texture.hh"
#include "../ray.hh"
#pragma once

namespace raytracing {
    class Object {
    public:
        Object(shared_texture texture, Vector3 position);
        virtual bool hit(const Ray&, HitRecord& hit_data) const = 0;
        shared_texture get_texture() const;
    protected:
        shared_texture texture;
        Vector3 position;
    };
    using shared_obj = std::shared_ptr<Object>;

    class Sphere : public Object {
    public:
        Sphere(shared_texture texture, const Vector3& center,
               float r);
        bool hit (const Ray& r, HitRecord& hit_data) const final;
    private:
        float radius;
    };
    class Square : public Object {
    public:
        Square(shared_texture texture, const Vector3& center,
              const Vector3& normal, const Vector3& up, float distance);
        bool hit (const Ray& r, HitRecord& hit_data) const final;
    private:
        Vector3 normal;
        Vector3 up;
        Vector3 right;
        float distance;
    };

}


#endif /* OBJECT_HH*/
