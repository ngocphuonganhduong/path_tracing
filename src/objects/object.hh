#ifndef OBJECT_HH
#define OBJECT_HH

#include "material.hh"
#include "../utils/ray.hh"
#include "../utils/vertex.hh"

namespace pathtracing {
    class Object {
        using shared_objs = std::vector<std::shared_ptr<Object>>;
    public:
        Object(const Vector3 &position, shared_mat mat);

        virtual bool hit(const Ray &, HitRecord &hit_data) const = 0;

        virtual double sampleDirectionPDF(const BSDFRecord& data) const = 0;
        virtual double sampleSurfacePositionPDF() const = 0;

        virtual Vector3 sampleSurfacePosition(double &pdf) const = 0;

        Ray sampleLightRay(double &pdfForward, double &pdfBackward) const;

        Vector3 phong_lighting(const Vector3 &li, const Vector3 &wi, const Vector3 &wo, const Vector3 &normal,
                               const double &d2);

        Vector3 position; //world position
        shared_mat material;
    };

    using shared_obj = std::shared_ptr<Object>;


    //SPHERE
    class Sphere : public Object {
    public:
        Sphere(const Vector3 &position, shared_mat mat, float radius);

        bool hit(const Ray &r, HitRecord &hit_data) const final;

        double sampleDirectionPDF(const BSDFRecord& data) const final;
        Vector3 sampleSurfacePosition(double &pdf) const final;

        double sampleSurfacePositionPDF() const;

    private:
        float radius;
    };


    //TRIANGLE MESH
    typedef struct {
        Vector3 p[3];
    } Triangle;

    Triangle operator*(const Triangle &v1, const Vector3 &scale);

    using TriVector = std::vector<Triangle>;

    class TriangleMesh : public Object {
    public:
        TriangleMesh(const Vector3 &position, shared_mat mat, TriVector &triangles,
                     const Vector3 &scale);


        double sampleSurfacePositionPDF() const;
        double sampleDirectionPDF(const BSDFRecord& data) const final;

        bool hit(const Ray &r, HitRecord &hit_data) const final;

        Vector3 sampleSurfacePosition(double &pdf) const final;

    private:
        bool hit(const Ray &r, HitRecord &hit_data,
                 const Triangle &tri) const;

        Vector3 scale;
        TriVector triangles;
    };

    //SQUARE
    class Square : public Object {
    public:
        Square(const Vector3 &position, shared_mat mat, const Vector3 &normal, const Vector3 &up, float halfSize);

        bool hit(const Ray &r, HitRecord &hit_data) const final;

        double sampleSurfacePositionPDF() const;
        double sampleDirectionPDF(const BSDFRecord& data) const final;

        Vector3 sampleSurfacePosition(double &pdf) const;


    private:

        Vector3 normal;
        Vector3 up;
        Vector3 right;
        float halfSize;
    };
}

#endif /* OBJECT_HH */
