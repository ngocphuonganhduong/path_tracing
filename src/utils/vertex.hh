#ifndef VERTEX_HH
#define VERTEX_HH

#include "vector3.hh"
#include <vector>


namespace pathtracing {

    struct HitRecord {
        unsigned int obj_id;
        Vector3 point;
        Vector3 normal;
    };

    struct Vertex {
        HitRecord data;
        Vector3 cumulative;
        Vector3 dir;

        double cumulative_pdf = 1.0; //used for calculating weights of bdpt

        Vertex(const HitRecord &data_) : data(data_) {}

        Vertex(const HitRecord &data_, const Vector3 &cumulative_, const double &cumulative_pdf_) : data(data_),
                                                                                                    cumulative(
                                                                                                            cumulative_),
                                                                                                    cumulative_pdf(
                                                                                                            cumulative_pdf_) {}
    };

    using Vertices = std::vector<Vertex>;
}
#endif /* VERTEX_HH */
