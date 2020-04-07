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

    enum BSDFTypeMask {
        DIFFUSE = 1 << 1,
        SPECULAR = 1 << 2,
        REFRACTION = 1 << 3,
    };

    class BSDF {
    public:
        BSDF(shared_mat mat) : mat_(mat) {
            if (mat_->ks.max() > 0) {
                type |= SPECULAR;
            }
            if (mat_->kd.max() > 0) {
                type |= DIFFUSE;
            }
        }


        Vector3 evalSampleBSDF(BSDFRecord &data, double &pdf) const;

        Vector3 evalBSDF(BSDFRecord &data, double &pdf) const;

        Vector3 f(const BSDFRecord &data, double &pdf) const;

        virtual double pdf(const BSDFRecord &data) const = 0;

        virtual Vector3 brdf(const BSDFRecord &data, double &pdf) const = 0;

//        virtual Vector3 evalBRDF(BSDFRecord &data, double &pdf) const = 0;

        Vector3 ka() const { return mat_->ka; }

        Vector3 ke() const { return mat_->ke; }
        Vector3 filter_opacity() const { return mat_->tf * (1- mat_->d); }

        double attenuation(const double &d2) const { return 1.0 / (mat_->a * d2 + mat_->b * sqrt(d2) + mat_->c); }

        bool is_light() const { return mat_->ke.max() > 0; }

        bool is_diffuse() const { return (type & DIFFUSE) == DIFFUSE; }

        static double computeFresnelReflectivity(double cosI, const double &n1, const double &n2);


    protected:
        virtual Vector3 sampleBRDF(BSDFRecord &data, double &pdf) const = 0;

        unsigned int type = 0;
        shared_mat mat_;
    };

    using shared_bsdf = std::shared_ptr<BSDF>;


    class DiffuseBSDF : public BSDF {
    public:
        DiffuseBSDF(shared_mat mat) : BSDF(mat) {
            type = DIFFUSE;
        }

        Vector3 sampleBRDF(BSDFRecord &data, double &pdf) const final;

        double pdf(const BSDFRecord &data) const final;
//

        Vector3 brdf(const BSDFRecord &data, double &pdf) const final;
//        Vector3 evalBRDF(BSDFRecord &data, double &pdf) const final;
    };

    class MirrorBSDF : public BSDF {
    public:
        MirrorBSDF(shared_mat mat) : BSDF(mat) {
            type = SPECULAR;
        }

        Vector3 sampleBRDF(BSDFRecord &data, double &pdf) const final;

        double pdf(const BSDFRecord &data) const final;

        Vector3 brdf(const BSDFRecord &data, double &pdf) const final;


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

        Vector3 sampleBRDF(BSDFRecord &data, double &pdf) const final;

        double pdf(const BSDFRecord &data) const final;

        Vector3 brdf(const BSDFRecord &data, double &pdf) const final;

    protected:
        double pd = 0.5;
        double ps = 0.5;
    };


    class BlinnPhongBSDF : public BSDF {
    public:
        BlinnPhongBSDF(shared_mat mat) : BSDF(mat) {
            Vector3 pds = mat_->kd + mat_->ks;
            double pr = pds.max();
            pd = mat_->kd.sum() / pds.sum();
            ps = mat_->ks.sum() / pds.sum();

            if (pr > 1) {
                std::cout << "[WARNING]: kd and ks violate the energy conservation. kd + ks must < 1\n";
            }
        }

        Vector3 sampleBRDF(BSDFRecord &data, double &pdf) const final;

        double pdf(const BSDFRecord &data) const final;

        Vector3 brdf(const BSDFRecord &data, double &pdf) const final;

    protected:
        double pd = 0.5;
        double ps = 0.5;
    };

    using shared_bsdf = std::shared_ptr<BSDF>;


}


#endif //bsdf_HH
