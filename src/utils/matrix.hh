//
// Created by rimuru on 28/03/2020.
//

#ifndef matrix_HH
#define matrix_HH

#include "vector3.hh"

namespace pathtracing {
    class Matrix3x3 {
    private:
        Vector3 rows[3];
    public:
        constexpr Matrix3x3() noexcept = default;

        constexpr Vector3 &operator[](unsigned int row) {
            return this->rows[row];
        }

        constexpr const double &operator()(unsigned int row, unsigned int col) const {
            return this->rows[row](col);
        }

        constexpr Vector3 operator*(const Vector3 &c) const noexcept {
            return rows[0] * c.x() + rows[1] * c.y() + rows[2] * c.z();
        }

        Vector3 mult_vector3(const Vector3 &v)
        {
            Vector3 res;
            
            for (int i = 0; i < 3; i++)
            {
                res[i] = rows[i].x() * v.x()
                      + rows[i].y() * v.y()
                      + rows[i].z() * v.z();
            }
            
            return res;
        }

        constexpr Matrix3x3 transpose() const {
            Matrix3x3 t_mat;
            const Matrix3x3 &mat = *this;

            for (unsigned i = 0; i < 3; ++i)
                for (unsigned j = 0; j < 3; ++j) {
                    t_mat.rows[i][j] = mat(j, i);
                }
            return t_mat;
        }

        friend std::ostream &operator<<(std::ostream &out, Matrix3x3 const &m) {
            return out << std::setprecision(2) << m.rows[0] << "\n" << m.rows[1] << "\n" << m.rows[2] << "\n";
        }

//        static Matrix3x3 worldToModel(const Vector3 &normal) {
//            Matrix3x3 mat; //(u, v, n) or (T (tangent), B(bi-tangent), N) space
//            mat[2] = normal;
//            mat[2].normalize();
//
//            Vector3 u;
//            if (fabs(normal.x()) > 0.1)
//                u = Vector3(0, 1, 0);
//            else
//                u = Vector3(1, 0, 0);
//            mat[0] = u.cross(normal);
//            mat[0].normalize();
//
//            mat[1] = mat[3].cross(u);
//
//            return mat;
//        }

        static Matrix3x3 modelToWorld(const Vector3 &normal) {
            Matrix3x3 mat; //(u, v, n) or (T (tangent), B(bi-tangent), N) space

            mat[2] = normal;
            Vector3 h = normal;
            if (fabs(h.x()) <= fabs(h.y()) && fabs(h.x()) <= fabs(h.z()))
                h[0] = 1.0;
            else if (fabs(h.y()) <= fabs(h.x()) && fabs(h.y()) <= fabs(h.z()))
                h[1] = 1.0;
            else
                h[2] = 1.0;

            mat[2].normalize();

            mat[1] = h.cross(mat[2]);
            mat[1].normalize();

            mat[0] = mat[2].cross(mat[1]);
            mat[0].normalize();

            return mat;
        }


    };

}


#endif //matrix_HH
