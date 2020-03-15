#ifndef VECTOR3_HH
#define VECTOR3_HH
#include <iostream>
#include <cmath>

#pragma once

namespace pathtracing {
    class Vector3
    {
    public:
        Vector3 ();
        Vector3 (double x_, double y_, double z_);

        ~Vector3(){};

        friend std::ostream& operator<<(std::ostream& out, Vector3 const& v);
        Vector3 operator+(const Vector3& v1) const;
        Vector3 operator-(const Vector3& v1) const;
        Vector3 operator*(const Vector3& v1) const;
        Vector3 operator*(const double& c) const;
        Vector3 operator/(const double& c) const;
        Vector3& operator+=(const Vector3& v);
        Vector3& operator*=(double c);
        Vector3& operator*=(const Vector3&);
        Vector3& operator/=(double c);
        Vector3& normalize();
        double dot(const Vector3& v) const;
        Vector3 cross(const Vector3& c) const;
        Vector3 reflect(const Vector3& normal) const;
        void clamp(double min, double max);
        void set_values(double x, double y, double z);
        double avg() const;
        double norm() const;
        double norm_square() const;
        double max() const;
        double x;
        double y;
        double z;
    };

    double rand1();
}

#endif /* VECTOR3_HH */
