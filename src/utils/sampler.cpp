//
// Created by rimuru on 26/03/2020.
//

#include "sampler.hh"

namespace pathtracing {
    Vector3 cosineSampleHemisphere(double &pdf) {
        double cosTheta = random_uniform();
        double sinTheta = sqrt(1 - cosTheta * cosTheta);
        double phi = 2.0 * M_PI * random_uniform();
        pdf = cosTheta * M_1_PI;
        return Vector3(sinTheta * cosf(phi), sinTheta * sinf(phi), cosTheta);

    }

    double cosineSampleHemispherePDF(const BSDFRecord &data) {
        return cos_theta(data.wo) * M_1_PI; //cos_theta / PI
    }

    /**
     * (u,v, normal)
     * Sampling direction around the normal, projected on the plane perpendicular to R
     * theta : angle between the normal and the output vector
     * phi : angle between the u axis  and the output vector
     * when n->infinity, cos(theta)->1, when theta->0 => the sharpest
     *
     * @param pdf
     * @param n
     * @return Direction vector
     */
    Vector3 cosinePowerSampleHemisphere(double &pdf, const double &n) {
        // specular + glossy
        // (x, y, z) = (sin(theta)cos(phi), sin(theta)sin(phi), cos(theta))
        double cosTheta = pow(drand48(), 1 / (n + 1));
        double sin_theta = sqrt(1 - cosTheta * cosTheta);
        double phi = 2 * M_PI * drand48();
        Vector3 wo (sin_theta * cos(phi), sin_theta * sin(phi), cosTheta);
        pdf = (n + 1.0)  * pow(cosTheta, n)/ (2.0 * M_PI);
        return wo;
    }

    double cosinePowerSampleHemispherePDF(const BSDFRecord &data, const double &n) {
        double cos_theta = Vector3::cos(data.wi.reflect_model_space(), data.wo);
        return (n + 1.0) * pow(cos_theta, n) * sqrt(1 - cos_theta * cos_theta)/ (2.0 * M_PI);
    }

    Vector3 uniformSampleSphere(double &pdf) {
        pdf = 0.5 / M_PI;
        float z = 1.0f - 2.0f * random_uniform();
        float sinTheta = sqrtf(std::max(0.0f, 1.0f - z * z));
        float phi = 2.0 * M_PI * random_uniform();
        float x = sinTheta * cos(phi);
        float y = sinTheta * sin(phi);
        return Vector3(x, y, z);
    }

    double uniformSampleSpherePDF() {
        return 0.5 / M_PI; //cos_theta = 0.5
    }


    Vector3 sampleHemisphere(double &pdf, const Vector3 &normal) {
        // orthogonal coordinate system (x=u, y=v, z=normal)
        //(x,y,z) = (cos(angle) * r, sin(angle) * r, sqrt(1 - r*r))
        pdf = 0.5 / M_PI; //  1 / (2pi)
        double angle = 2 * M_PI * drand48();
        double r = sqrt(drand48());
        Vector3 u;
        if (fabs(normal.x()) > 0.1)
            u = Vector3(0, 1, 0);
        else
            u = Vector3(1, 0, 0);
        u = u.cross(normal);
        u.normalize();

        Vector3 v = normal.cross(u);

        return (u * cos(angle) * r + v * sin(angle) * r +
                normal * sqrt(1 - r * r)).normalize();
    }


}
