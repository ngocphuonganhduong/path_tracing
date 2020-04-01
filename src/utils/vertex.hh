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
        Vector3 li;
        Vector3 wi; //incoming light direction
        HitRecord data;

        Vertex(const Vector3 &li_, const HitRecord &data_) : li(li_), data(data_) {}
        Vertex(const Vector3 &li_, const Vector3 &wi_, const HitRecord &data_) : li(li_), wi(wi_), data(data_) {}

    };

    using Path = std::vector<Vertex>;
    using Paths = std::vector<Path>;
}
#endif /* VERTEX_HH */
