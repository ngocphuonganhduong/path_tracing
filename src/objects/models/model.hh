#ifndef MODEL_HH
#define MODEL_HH
#include "../../utils/ray.hh"
#include <memory>
#include <vector>

namespace pathtracing {
    class Model {
    public:
        virtual bool hit(const Vector3& pos, const Ray& r,
                         HitRecord& hit_data) const = 0;
        virtual Vector3 get_sample(const Vector3& pos) const = 0;
    };
    using shared_mod = std::shared_ptr<Model>;


    //SPHERE
    class Sphere : public Model {
    public:
        Sphere(float radius);
        bool hit (const Vector3& pos, const Ray& r,
                  HitRecord& hit_data) const final;
        Vector3 get_sample(const Vector3& pos) const final;
    private:
        float radius;
    };


    //TRIANGLE MESH
    typedef struct { Vector3 p[3];} Triangle;
    Triangle operator*(const Triangle& v1, const Vector3& scale);
    using TriVector = std::vector<Triangle>;

    class TriangleMesh : public Model {
    public:
        TriangleMesh(TriVector& triangles,
                     const Vector3& scale);

        bool hit (const Vector3& pos, const Ray& r,
                  HitRecord& hit_data) const final;

        Vector3 get_sample(const Vector3& pos) const final;

    private:
        bool hit (const Vector3& pos, const Ray& r, HitRecord& hit_data,
                  const Triangle& tri) const;

        // real pos = pos + tri.pos;
        Vector3 scale;
        TriVector triangles;
    };

    //SQUARE
    class Square : public Model {
    public:
        Square(const Vector3& normal, const Vector3& up, float distance);
        bool hit (const Vector3& pos, const Ray& r,
                  HitRecord& hit_data) const final;
        Vector3 get_sample(const Vector3& pos) const final;
    private:
        Vector3 normal;
        Vector3 up;
        Vector3 right;
        float distance;
    };
}

#endif /* MODEL_HH */
