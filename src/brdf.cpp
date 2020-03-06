#include "brdf.hh"

namespace pathtracing {
    Vector3 sample_diffuse(const Vector3& normal) {
        // orthogonal coordinate system (x=u, y=v, z=normal)
        //(x,y,z) = (cos(angle) * r, sin(angle) * r, sqrt(1 - r*r))
        double angle = 2 * M_PI * drand48();
        double r = drand48();
        Vector3 u;
        if (fabs(normal.x) > 0.1)
            u = Vector3(0, 1, 0);
        else
            u = Vector3(1, 0, 0);
        u = u.cross(normal);
        Vector3 v = normal.cross(u);
        return (u * cos(angle) * r + v * sin(angle) * r +
                normal * sqrt(1 - r*r)).normalize();
    }

}
