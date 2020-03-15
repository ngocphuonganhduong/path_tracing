#ifndef VERTEX_HH
#define VERTEX_HH

#include "vector3.hh"
#include <vector>


namespace pathtracing {
    struct Vertex {
        Vector3 pos;
        unsigned int obj_id;
        Vector3 col;
        Vertex(const Vector3& position, unsigned int obj_id_,
               const Vector3& col_)
            : pos(position), obj_id(obj_id_), col(col_){}
    };
    using Vertices = std::vector<Vertex>;

}
#endif /* VERTEX_HH */
