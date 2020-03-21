#include "brdf.hh"

namespace pathtracing {
    Vector3 sample_diffuse(const Vector3& normal) {
        // orthogonal coordinate system (x=u, y=v, z=normal)
        //(x,y,z) = (cos(angle) * r, sin(angle) * r, sqrt(1 - r*r))
        double angle = 2 * M_PI * drand48();
        double r = sqrt(drand48());
        Vector3 u;
        if (fabs(normal.x) > 0.1)
            u = Vector3(0, 1, 0);
        else
            u = Vector3(1, 0, 0);
        u = u.cross(normal);
        u.normalize();

        Vector3 v = normal.cross(u);

        return (u * cos(angle) * r + v * sin(angle) * r +
                normal * sqrt(1 - r*r)).normalize();
    }

    Vector3 sample_specular(const Vector3& normal, float n) {
        // specular + glossy
        // (x, y, z) = (sin(alpha)cos(phi), sin(alpha)sin(phi), cos(alpha))
        // alpha : angle between specular reflective and lightning directions
        // phi : angle between the specular reflective direction (R) and the
        //       sample's projection on the plane perpendicular to R

        double alpha = acos(pow(drand48(), 1 / (n + 1)));
        double phi = 2 * M_PI * drand48();

        Vector3 u;
        if (fabs(normal.x) > 0.1)
            u = Vector3(0, 1, 0);
        else
            u = Vector3(1, 0, 0);
        u = u.cross(normal);
        Vector3 v = normal.cross(u);
        
        return (u * sin(alpha) * cos(phi) + v * sin(alpha) * sin(phi) +
                normal * cos(alpha)).normalize();
    }
}
