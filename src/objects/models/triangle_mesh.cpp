# include <cmath>

# include "../object.hh"
# include "../../utils/sampler.hh"

# define _USE_MATH_DEFINES

namespace pathtracing {

    void Triangle::scale(const Vector3 &scale)
    {
        p[0] = p[0] * scale;
        p[1] = p[1] * scale;
        p[2] = p[2] * scale;
    }

    void Triangle::rotate(const Vector3 &rot)
    {
        // rotation = (gamma, beta, alpha) Euler angles (= x,y,z as it's a Vector3)
        // with R = Rz(alpha) * Ry(beta) * Rx(gamma)
        //          |cos(alpha)cos(beta)  cos(alpha)sin(beta)sin(gamma)-sin(alpha)cos(gamma)  cos(alpha)sin(beta)cos(gamma)+sin(alpha)sin(gamma)|
        //      R = |sin(alpha)sin(beta)  sin(alpha)sin(beta)sin(gamma)+cos(alpha)cos(gamma)  sin(alpha)sin(beta)cos(gamma)-cos(alpha)sin(gamma)|
        //          |  -sin(beta)                   cos(beta)sin(gamma)                                     cos(beta)cos(gamma)                 |

        // Convert degrees to radians
        Vector3 rad(rot.z() * M_PI / 180, rot.y() * M_PI / 180, rot.z() * M_PI / 180);

        Matrix3x3 R;
        R[0] = Vector3(cos(rad.z()) * cos(rad.y()),
                       cos(rad.z()) * sin(rad.y()) * sin(rad.x()) - sin(rad.z()) * cos(rad.x()),
                       cos(rad.z()) * sin(rad.y()) * cos(rad.x()) + sin(rad.z()) * sin(rad.x()));
        
        R[1] = Vector3(sin(rad.z()) * sin(rad.y()),
                       sin(rad.z()) * sin(rad.y()) * sin(rad.x()) + cos(rad.z()) * cos(rad.x()),
                       sin(rad.z()) * sin(rad.y()) * cos(rad.x()) - cos(rad.z()) * sin(rad.x()));

        R[2] = Vector3(-1 * sin(rad.y()),
                       cos(rad.y()) * sin(rad.x()),
                       cos(rad.y()) * cos(rad.x()));

        p[0] = R.mult_vector3(p[0]);
        p[1] = R.mult_vector3(p[1]);
        p[2] = R.mult_vector3(p[2]);
    }

    TriangleMesh::TriangleMesh(const Vector3 &position, shared_bsdf bsdf, TriVector &triangles_, const Vector3 &scale, const Vector3 &rotation)
            : Object(position, bsdf), triangles(triangles_)
    {
        for (auto tri = triangles.begin(); tri != triangles.end(); ++tri)
        {
            // Apply scaling
            (*tri).scale(scale);

            // Apply rotation
            (*tri).rotate(rotation);
        }
    }

//
//    TriangleMesh::TriangleMesh(const Vector3 &position, shared_mat mat, const Vector3 &color,
//                               TriVector &triangles_, const Vector3 &scale_)
//            : Object(position, mat, color), scale(scale_), triangles(triangles_) {}


    bool TriangleMesh::hit(const Ray &r, HitRecord &hit_data, const Triangle &tri) const {
        const double EPSILON = 0.0001;

        Vector3 p0 = tri.p[0];
        Vector3 p1 = tri.p[1];
        Vector3 p2 = tri.p[2];

        Vector3 edge1 = p1 - p0;
        Vector3 edge2 = p2 - p0;

        Vector3 h = r.get_direction().cross(edge2);
        float a = edge1.dot(h);
        if (a < -EPSILON || a > EPSILON) {
            float f = 1.0 / a;
            Vector3 s = r.get_origin() - position - p0;
            float u = f * s.dot(h);

            if (u >= 0 && u <= 1) {
                Vector3 q = s.cross(edge1);
                float v = f * r.get_direction().dot(q);
                if (v >= 0.0 && u + v <= 1.0) {
                    float t = f * edge2.dot(q);
                    if (t > EPSILON) {
                        hit_data.point = r.get_origin() + r.get_direction() * t;
                        hit_data.normal = edge1.cross(edge2);
                        hit_data.normal.normalize();
//                        hit_data.direction = hit_data.point - r.get_origin();
                        return true;
                    }

                }
            }
        }
        return false;
    }

    bool TriangleMesh::hit(const Ray &r, HitRecord &hit_data) const {
        bool hit = false;
        HitRecord tem;
        float min_ds = INFINITY;
        float ds = 0;

        for (auto tri: this->triangles) {
            if (this->hit(r, tem, tri)) {
                ds = (tem.point - r.get_origin()).norm_square();
                if (!hit || ds < min_ds) {
                    hit_data = tem;
                    min_ds = ds;
                }
                hit = true;
            }
        }
        return hit;
    }

    Vector3 TriangleMesh::sampleSurfacePosition(double &pdf, Vector3 &surfaceNormal) const {
        pdf = sampleSurfacePositionPDF();
        auto tri = triangles[round(drand48() * (triangles.size() - 1))];
        surfaceNormal = (tri.p[1] - tri.p[0]).cross(tri.p[2] - tri.p[0]);
        surfaceNormal.normalize();
        return position + tri.p[0];
    }

    double TriangleMesh::sampleSurfacePositionPDF() const {
        return 1.0 / triangles.size();
    }

    double TriangleMesh::sampleDirectionPDF(const BSDFRecord &) const {
        return uniformSampleSpherePDF();
    }
}
