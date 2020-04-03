//
// Created by rimuru on 29/03/2020.
//

#ifndef BSDF_HH
#define BSDF_HH

#include <memory>
//#include "../../utils/vertex.hh"
#include "../../utils/matrix.hh"
#include "../../utils/sampler.hh"
#include "../material.hh"

namespace pathtracing {

    double computeFresnelReflectivity(double cosI, const double &n1, const double &n2);

    class BSDF {
    public:
        BSDF(shared_mat mat) : mat_(mat) {}

        virtual Vector3 sampleBSDF(BSDFRecord &data, double &pdf) const = 0;

        virtual double pdf(const BSDFRecord &data) const = 0;

        virtual Vector3 f(const BSDFRecord &data) const = 0;

        Vector3 ka() const { return mat_->ka; }

        Vector3 ke() const { return mat_->ke; }

        double attenuation(const double &d2) const { return mat_->a * d2 + mat_->b * sqrt(d2) + mat_->c; }

        bool is_light() const { return mat_->ke.max() > 0; }

    protected:
        shared_mat mat_;
    };

    using shared_bsdf = std::shared_ptr<BSDF>;


    class DiffuseBSDF : public BSDF {
    public:
        DiffuseBSDF(shared_mat mat) : BSDF(mat) {}

        Vector3 sampleBSDF(BSDFRecord &data, double &pdf) const final;

        double pdf(const BSDFRecord &data) const final;

        Vector3 f(const BSDFRecord &data) const final;

    };

    class MirrorBSDF : public BSDF {
    public:

        Vector3 sampleBSDF(BSDFRecord &data, double &pdf) const final;

        double pdf(const BSDFRecord &data) const final;

        Vector3 f(const BSDFRecord &data) const final;


    };


    class PhongBSDF : public BSDF {
    public:
        PhongBSDF(shared_mat mat) : BSDF(mat) {
            Vector3 pds = mat_->kd + mat_->ks;
            double pr = pds.max();
            pd = mat_->kd.sum() / pds.sum();
            ps = pr - pd;

            if (pd + ps > 1) {
                std::cout << "[WARNING]: kd and ks violate the energy conservation. kd + ks must < 1\n";
            }
        }

        Vector3 sampleBSDF(BSDFRecord &data, double &pdf) const;

        double pdf(const BSDFRecord &data) const;

        Vector3 f(const BSDFRecord &data) const;

    protected:
        double pd = 0.5;
        double ps = 0.5;
    };


    class BlinnPhongBSDF : public PhongBSDF {
    public:
        BlinnPhongBSDF(shared_mat mat) : PhongBSDF(mat) {}

        Vector3 sampleBSDF(BSDFRecord &data, double &pdf) const final;

        double pdf(const BSDFRecord &data) const final;

        Vector3 f(const BSDFRecord &data) const final;
    };

    using shared_bsdf = std::shared_ptr<BSDF>;


}


#endif //bsdf_HH
