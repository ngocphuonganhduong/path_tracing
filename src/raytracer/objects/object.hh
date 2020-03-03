#ifndef OBJECT_HH
#define OBJECT_HH
#include "texture.hh"
#include "../ray.hh"
#include <vector>

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

    //SPHERE
    class Sphere : public Object {
    public:
        Sphere(shared_texture texture, const Vector3& center,
               float r);
        bool hit (const Ray& r, HitRecord& hit_data) const final;
    private:
        float radius;
    };

    //SQUARE
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


    //TRIANGLE MESH
    typedef struct { Vector3 p[3];} Triangle;
    Triangle operator*(const Triangle& v1, const Vector3& scale);

    using TriVector = std::vector<Triangle>;

    class TriangleMesh : public Object {
    public:
        TriangleMesh(shared_texture texture, const Vector3& pos,
                     TriVector& triangles,
                     const Vector3& scale);

        bool hit (const Ray& r, HitRecord& hit_data,
                  const Triangle& tri) const ;
        bool hit (const Ray& r, HitRecord& hit_data) const final;
    private:
        // real pos = pos + tri.pos;
        Vector3 scale;
        TriVector triangles;
    };

}


#endif /* OBJECT_HH*/
