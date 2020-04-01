#ifndef VECTOR3_HH
#define VECTOR3_HH

#include <iostream>
#include <cmath>
#include <optional>
#include <algorithm>
#include <iomanip>

#pragma once

namespace pathtracing {
    class Vector3 {
    private:
        double val[3]{0,0,0};

    public:
        constexpr double x() const noexcept {
            return val[0];
        }

        constexpr double y() const noexcept {
            return val[1];
        }

        constexpr double z() const noexcept {
            return val[2];
        }

        constexpr Vector3() noexcept = default;

        constexpr Vector3(double x, double y, double z) noexcept : val{x, y, z} {}

        constexpr Vector3(double c) noexcept : val{c, c, c} {}

        constexpr const double& operator()(unsigned int col) const {
            return val[col];
        }
        constexpr double& operator[](unsigned int col) {
            return val[col];
        }

        friend std::ostream &operator<<(std::ostream &out, Vector3 const &v) {
            return out << std::setprecision(2) << "(" << v.val[0] << ", " << v.val[1]
                       << ", " << v.val[2] << ")";
        }

        constexpr Vector3 operator+(const Vector3 &v) const noexcept {
            return Vector3(val[0] + v.val[0], val[1] + v.val[1], val[2] + v.val[2]);
        }

        constexpr Vector3 operator-(const Vector3 &v) const noexcept {
            return Vector3(val[0] - v.val[0], val[1] - v.val[1], val[2] - v.val[2]);
        }

        constexpr Vector3 operator*(const Vector3 &v) const noexcept {
            return Vector3(val[0] * v.val[0], val[1] * v.val[1], val[2] * v.val[2]);
        }

        constexpr Vector3 operator*(const double &c) const noexcept {
            return Vector3(val[0] * c, val[1] * c, val[2] * c);
        }

        constexpr Vector3 operator/(const double &c) const noexcept {
            return Vector3(val[0] / c, val[1] / c, val[2] / c);
        }

        constexpr Vector3 &operator+=(const Vector3 &v) noexcept {
            this->val[0] += v.val[0];
            this->val[1] += v.val[1];
            this->val[2] += v.val[2];
            return *this;
        }

        constexpr Vector3 &operator-=(const Vector3 &v) noexcept {
            this->val[0] -= v.val[0];
            this->val[1] -= v.val[1];
            this->val[2] -= v.val[2];
            return *this;
        }

        constexpr Vector3 &operator*=(double c) noexcept {
            this->val[0] *= c;
            this->val[1] *= c;
            this->val[2] *= c;
            return *this;
        }

        constexpr Vector3 &operator*=(const Vector3 &v) noexcept {
            this->val[0] *= v.val[0];
            this->val[1] *= v.val[1];
            this->val[2] *= v.val[2];
            return *this;
        }

        constexpr Vector3 &operator/=(double c) noexcept {
            this->val[0] /= c;
            this->val[1] /= c;
            this->val[2] /= c;
            return *this;
        }

        Vector3 &normalize() noexcept {
            *this /= this->norm();
            return *this;
        }

        constexpr double sum() const noexcept {
            return val[0] + val[1] + val[2];
        }

        constexpr double dot(const Vector3 &v) const noexcept {
            return val[0] * v.val[0] + val[1] * v.val[1] + val[2] * v.val[2];
        }

        constexpr Vector3 cross(const Vector3 &v) const noexcept {
            return Vector3(val[1] * v.val[2] - val[2] * v.val[1], val[2] * v.val[0] - val[0] * v.val[2],
                           val[0] * v.val[1] - val[1] * v.val[0]);
        }

        constexpr Vector3 reflect(const Vector3 &normal) const noexcept {
            return *this - normal * normal.dot(*this) * 2.0;
        }

        constexpr Vector3 reflect_model_space() const noexcept {
            return Vector3(-val[0], -val[1], val[2]);;
        }

//        constexpr std::optional<Vector3> refract(const Vector3 &normal,
//                                                 double n) const noexcept {
//            const double cosI = -normal.dot(*this);
//            const double sinT2 = n * n * (1 - cosI * cosI);
//            if (sinT2 > 1)
//                return std::nullopt;
//            const double cosT = sqrt(1 - sinT2);
//            return *this * n + normal * (n * cosI - cosT);
//        }

        constexpr void clamp(double min, double max) noexcept {
            val[0] = std::clamp(val[0], min, max);
            val[1] = std::clamp(val[1], min, max);
            val[2] = std::clamp(val[2], min, max);
        }

        constexpr double avg() const noexcept {
            return (val[0] + val[1] + val[2]) / 3;
        }

        double norm() const noexcept {
            return sqrt(this->norm_square());
        }

        constexpr double norm_square() const noexcept {
            return val[0] * val[0] + val[1] * val[1] + val[2] * val[2];
        }

        constexpr double max() const noexcept {
            return std::max(val[0], std::max(val[1], val[2]));
        }

        static double cos(const Vector3 &v1, const Vector3 &v2) noexcept {
            return v1.dot(v2) / (v1.norm() * v2.norm());
        }
    };

//    double rand1();
}

#endif /* VECTOR3_HH */
