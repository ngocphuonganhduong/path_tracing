#ifndef OBJECT_HH
#define OBJECT_HH

#include "material.hh"
#include "../utils/ray.hh"
#include "../utils/vertex.hh"
#include "bsdf/bsdf.hh"

namespace pathtracing {
    class Object {
        using shared_objs = std::vector<std::shared_ptr<Object>>;
    public:
        Object(const Vector3 &position, shared_bsdf bsdf);

        virtual bool hit(const Ray &, HitRecord &hit_data) const = 0;

        virtual double sampleDirectionPDF(const BSDFRecord &data) const = 0;

        virtual double sampleSurfacePositionPDF(const HitRecord &hit_data) const = 0;

        virtual Vector3 sampleSurfacePosition(double &pdf, Vector3& normal) const = 0;

        virtual Vector3 Le(const Vector3 &pos, const Vector3 &wo) const;


//        Ray sampleLightRay(double &pdfForward, double &pdfBackward) const;
//
//        Vector3 phong_lighting(const Vector3 &li, const Vector3 &wi, const Vector3 &wo, const Vector3 &normal,
//                               const double &d2);

        Vector3 position; //world position
        shared_bsdf bsdf;
    };

    using shared_obj = std::shared_ptr<Object>;


    //SPHERE
    class Sphere : public Object {
    public:
        Sphere(const Vector3 &position, shared_bsdf bsdf, float radius);

        bool hit(const Ray &r, HitRecord &hit_data) const final;

        double sampleDirectionPDF(const BSDFRecord &data) const final;

        Vector3 sampleSurfacePosition(double &pdf, Vector3& normal) const final;

        double sampleSurfacePositionPDF(const HitRecord &hit_data) const;

    private:
        float radius;
    };


    //TRIANGLE MESH
    typedef struct {
        Vector3 p[3];
        double area;
    
        void scale(const Vector3 &scale);
        void rotate(const Vector3 &rotation);
        
        void compute_area();

        double sampleSurfacePositionPDF() const;
        Vector3 sampleSurfacePosition(double &pdf, Vector3& normal);

    } Triangle;

    using TriVector = std::vector<Triangle>;

    class TriangleMesh : public Object {
    public:
        TriangleMesh(const Vector3 &position, shared_bsdf bsdf, TriVector &triangles,
                     const Vector3 &scale, const Vector3 &rotation);

        double sampleSurfacePositionPDF(const HitRecord &hit_data) const;

        double sampleDirectionPDF(const BSDFRecord &data) const final;

        bool hit(const Ray &r, HitRecord &hit_data) const final;

        Vector3 sampleSurfacePosition(double &pdf, Vector3& normal) const final;

    private:
        bool hit(const Ray &r, HitRecord &hit_data,
                 const Triangle &tri) const;

        TriVector triangles;
    };

    //SQUARE
    class Square : public Object {
    public:
        Square(const Vector3 &position, shared_bsdf bsdf, const Vector3 &normal, const Vector3 &up, float halfSize);

        bool hit(const Ray &r, HitRecord &hit_data) const final;

        double sampleSurfacePositionPDF(const HitRecord &hit_data) const;

        double sampleDirectionPDF(const BSDFRecord &data) const final;

        Vector3 sampleSurfacePosition(double &pdf, Vector3& normal) const;

        Vector3 Le(const Vector3 &pos, const Vector3 &wo) const final;


    private:

        Vector3 normal;
        Vector3 up;
        Vector3 right;
        float halfSize;
    };
}

#endif /* OBJECT_HH */
