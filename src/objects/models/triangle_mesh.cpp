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
        Vector3 rad(rot.x() * M_PI / 180, rot.y() * M_PI / 180, rot.z() * M_PI / 180);

        Matrix3x3 R;
        R[0] = Vector3(cos(rad.z()) * cos(rad.y()),
                       cos(rad.z()) * sin(rad.y()) * sin(rad.x()) - sin(rad.z()) * cos(rad.x()),
                       cos(rad.z()) * sin(rad.y()) * cos(rad.x()) + sin(rad.z()) * sin(rad.x()));
        
        R[1] = Vector3(sin(rad.z()) * cos(rad.y()),
                       sin(rad.z()) * sin(rad.y()) * sin(rad.x()) + cos(rad.z()) * cos(rad.x()),
                       sin(rad.z()) * sin(rad.y()) * cos(rad.x()) - cos(rad.z()) * sin(rad.x()));

        R[2] = Vector3(-1 * sin(rad.y()),
                       cos(rad.y()) * sin(rad.x()),
                       cos(rad.y()) * cos(rad.x()));

        p[0] = R.mult_vector3(p[0]);
        p[1] = R.mult_vector3(p[1]);
        p[2] = R.mult_vector3(p[2]);
    }

    void Triangle::compute_area()
    {
        Vector3 base = p[1] - p[0];
        Vector3 height = p[2] - p[0];
        area = 0.5 * (base.cross(height)).norm();
    }

    double Triangle::sampleSurfacePositionPDF() const
    {
        return 1.0 / area;
    }

    Vector3 Triangle::sampleSurfacePosition(double &pdf, Vector3& normal)
    {
        pdf = sampleSurfacePositionPDF();
        normal = (p[1] - p[0]).cross(p[2] - p[0]);
        normal.normalize();

        double r = drand48();
        double s = drand48();

        if (r + s >= 1.0)
        {
            r = 1 - r;
            s = 1 - s;
        }

        return p[0] + (p[1] - p[0]) * r + (p[2] - p[0]) * s;
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

            // Compute the triangle's area
            (*tri).compute_area();
        }

        last_hit_triangle_idx = 0;
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

    bool TriangleMesh::hit(const Ray &r, HitRecord &hit_data) {
        bool hit = false;
        HitRecord tem;
        float min_ds = INFINITY;
        float ds = 0;

        for (unsigned tri_idx = 0; tri_idx < triangles.size(); tri_idx++)
        {
            if (this->hit(r, tem, triangles[tri_idx])) {
                ds = (tem.point - r.get_origin()).norm_square();
                if (!hit || ds < min_ds) {
                    hit_data = tem;
                    min_ds = ds;
                    last_hit_triangle_idx = tri_idx;
                }
                hit = true;
            }
        }
        return hit;
    }

    Vector3 TriangleMesh::sampleSurfacePosition(double &pdf, Vector3 &surfaceNormal) const
    {
        auto tri = triangles[round(drand48() * (triangles.size() - 1))];
        return position + tri.sampleSurfacePosition(pdf, surfaceNormal);
    }

    double TriangleMesh::sampleSurfacePositionPDF() const
    {
        auto tri = triangles[last_hit_triangle_idx];
        return tri.sampleSurfacePositionPDF();
    }

    /*Vector3 TriangleMesh::sampleSurfacePosition(double &pdf, Vector3 &surfaceNormal) const {
        pdf = sampleSurfacePositionPDF();
        auto tri = triangles[round(drand48() * (triangles.size() - 1))];
        surfaceNormal = (tri.p[1] - tri.p[0]).cross(tri.p[2] - tri.p[0]);
        surfaceNormal.normalize();
        return position + tri.p[unsigned (floor(drand48() * 3))];
    }

    double TriangleMesh::sampleSurfacePositionPDF() const {
        return 1.0 / (triangles.size() * 3);
    }*/

    double TriangleMesh::sampleDirectionPDF(const BSDFRecord &) const {
        return uniformSampleSpherePDF();
    }
}
