#include <iomanip>
#include <cmath>
#include "vector3.hh"

namespace raytracing {
    Vector3::Vector3() :x(0), y(0), z(0){}

    Vector3::Vector3(double x_, double y_, double z_) :x(x_), y(y_), z(z_) {}

    void Vector3::set_values(double x_, double y_, double z_) {
        this->x = x_;
        this->y = y_;
        this->z = z_;
    }
    std::ostream& operator<<(std::ostream& out, Vector3 const& v){
        return out << std::setprecision(2) << "(" << v.x << ", " << v.y
                   << ", " << v.z << ")";
    }

    Vector3 Vector3::operator+(const Vector3& v) const {
        return Vector3(x + v.x, y + v.y, z + v.z);
    }

    Vector3 Vector3::operator-(const Vector3& v) const {
        return Vector3(x - v.x, y - v.y, z - v.z);
    }

    Vector3 Vector3::operator*(const Vector3& v) const {
        return Vector3(x * v.x, y * v.y, z * v.z);
    }

    Vector3 Vector3::operator*(const double& c) const {
        return Vector3(x * c, y * c, z * c);
    }

    Vector3 Vector3::operator/(const double& c) const {
        return Vector3(x / c, y / c, z / c);
    }

    Vector3& Vector3::operator+=(const Vector3& v) {
        this->x += v.x;
        this->y += v.y;
        this->z += v.z;
        return *this;
    }
    Vector3& Vector3::operator*=(double c) {
        this->x_ *= c;
        this->y_ *= c;
        this->z_ *= c;
        return *this;
    }
    Vector3& Vector3::operator*=(const Vector3& v) {
        this->x *= v.x;
        this->y *= v.y;
        this->z *= v.z;
        return *this;
    }
    Vector3& Vector3::operator/=(double c) {
        this->x /= c;
        this->y /= c;
        this->z /= c;
        return *this;
    }
    void Vector3::normalize(){
        *this /= this->norm();
    }

    double Vector3::dot(const Vector3& v) const{
        return x * v.x + y * v.y + z * v.z;
    }

    Vector3 Vector3::cross_product(const Vector3& v) const {
        return Vector3(y * v.z - z * v.y, z * v.x - x * v.z,
                       x * v.y - y * v.x);
    }
    double Vector3::norm() const {
        return sqrt(this->norm_square());
    }
    double norm_square() const {
        return x * x + y * y + z * z;
    }

    Vector3 Vector3::reflect(const Vector3& normal) const
    {
        return *this - normal * normal.dot(*this) * 2.0;
    }
    double rand1() {
        return double(rand()) / (double(RAND_MAX) + 1.0);
    }

}
