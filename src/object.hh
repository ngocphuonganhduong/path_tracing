#ifndef OBJECT_HH
#define OBJECT_HH
#include "ray.hh"
#include "texture.hh"

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
/*
    class Plane : public Object {
    public:
        Plane(shared_texture texture, const Vector3& point, const Vector3& normal);
        bool hit (const Ray& r, HitRecord& hit_data) const final;
    private:
        const Vector3& normal;
    };
*/
    class Square : public Object {
    public:
        Square(shared_texture texture, const Vector3& center,
              const Vector3& normal, const Vector3& up, float distance);
        bool hit (const Ray& r, HitRecord& hit_data) const final;
//        float get_smooth_step(const Ray& r) const final;
    private:
        Vector3 normal;
        Vector3 up;
        Vector3 right;
        float distance;
    };

}


#endif /* OBJECT_HH*/
