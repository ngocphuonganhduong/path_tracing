#include "model.hh"

namespace pathtracing {

    TriangleMesh::TriangleMesh(TriVector& triangles_, const Vector3& scale_)
        : scale(scale_), triangles(triangles_) {}

    bool TriangleMesh::hit (const Vector3& pos, const Ray& r,
                            HitRecord& hit_data, const Triangle& tri) const
    {
        const double EPSILON = 0.0001;

        Vector3 p0 = tri.p[0] * scale;
        Vector3 p1 = tri.p[1] * scale;
        Vector3 p2 = tri.p[2] * scale;

        Vector3 edge1 = p1 - p0;
        Vector3 edge2 = p2 - p0;

        Vector3 h = r.get_direction().cross_product(edge2);
        float a = edge1.dot(h);
        if (a < -EPSILON || a > EPSILON) {
            float f = 1.0 / a;
            Vector3 s = r.get_origin() - pos - p0;
            float u = f * s.dot(h);

            if (u >= 0 && u <= 1) {
                Vector3 q = s.cross_product(edge1);
                float v = f * r.get_direction().dot(q);
                if (v >= 0.0 && u + v <= 1.0) {
                    float t = f * edge2.dot(q);
                    if (t > EPSILON) {
                        hit_data.point = r.get_origin() + r.get_direction() * t;
                        hit_data.normal = edge1.cross_product(edge2);
                        hit_data.normal.normalize();
                        hit_data.direction = hit_data.point - r.get_origin();
                        return true;
                    }

                }
            }
        }
        return false;
    }

    bool TriangleMesh::hit (const Vector3& pos, const Ray& r,
                            HitRecord& hit_data) const
    {
        bool hit = false;
        HitRecord tem;
        float min_ds = INFINITY;
        float ds = 0;

        for (auto tri: this->triangles) {
            if (this->hit(pos, r, tem, tri))
            {
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
}
