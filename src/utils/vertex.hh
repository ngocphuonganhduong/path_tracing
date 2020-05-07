#ifndef VERTEX_HH
#define VERTEX_HH

#include "vector3.hh"
#include <vector>


namespace pathtracing {

    struct HitRecord {
        unsigned int obj_id;
        unsigned int tri_idx; //if the object is a triangle mesh, store the triangle idx
        Vector3 point;
        Vector3 normal;
    };

    struct Vertex {
        HitRecord data;
        Vector3 cumulative;
        Vector3 dir;
        double mis_ratio = 1.0; //p(i-1) / p(i)
        bool is_emissive = false;
        double d2 = 1.0;
        double backward_pdf_G = 1.0; //used for calculating weights of bdpt

        Vertex(const HitRecord &data_) : data(data_) {}

        Vertex(const HitRecord &data_, const Vector3 &cumulative_,
               const double &backward_G_) : data(data_), cumulative(cumulative_),
                                           backward_pdf_G(backward_G_) {}
    };

    using Vertices = std::vector<Vertex>;
}
#endif /* VERTEX_HH */
