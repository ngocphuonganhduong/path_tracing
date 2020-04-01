//
// Created by rimuru on 29/03/2020.
//

#ifndef BSDF_HH
#define BSDF_HH

#include <memory>
//#include "../../utils/vertex.hh"
#include "../../utils/matrix.hh"
#include "../../utils/sampler.hh"

namespace pathtracing {


    class BSDF {
    public:

        virtual Vector3 sampleBSDF(BSDFRecord &data, double &pdf) const = 0;

        virtual double pdf(const BSDFRecord &data) const = 0;

        virtual Vector3 f(const BSDFRecord &data) const = 0;

    };

    using shared_bsdf = std::shared_ptr<BSDF>;


    class DiffuseBSDF : public BSDF {
    public:
        DiffuseBSDF(const Vector3 &kd) : kd_(kd) {}

        Vector3 sampleBSDF(BSDFRecord &data, double &pdf) const final;

        double pdf(const BSDFRecord &data) const final;

        Vector3 f(const BSDFRecord &data) const final;

    private:
        Vector3 kd_;
    };

    class MirrorBSDF : public BSDF {
    public:
        MirrorBSDF(const Vector3 &ks) : ks_(ks) {}

        Vector3 sampleBSDF(BSDFRecord &data, double &pdf) const final;

        double pdf(const BSDFRecord &data) const final;

        Vector3 f(const BSDFRecord &data) const final;

    private:
        Vector3 ks_;
    };


    class PhongBSDF : public BSDF {
    public:
        PhongBSDF(const Vector3 &kd, const Vector3 &ks, const double &ns) : kd_(kd), ks_(ks), ns_(ns) {
            pd = kd.max();
            ps = ks.max();
            double s = pd + ps;
            pd /= s;
            ps /= s;
        }

        Vector3 sampleBSDF(BSDFRecord &data, double &pdf) const;

        double pdf(const BSDFRecord &data) const;

        Vector3 f(const BSDFRecord &data) const;

    protected:
        double pd = 0.5;
        double ps = 0.5;
        Vector3 kd_;
        Vector3 ks_;
        double ns_;
    };


    class BlinnPhongBSDF : public PhongBSDF {
    public:
        BlinnPhongBSDF(const Vector3 &kd, const Vector3 &ks, const double &ns) : PhongBSDF(kd, ks, ns) {}

        Vector3 sampleBSDF(BSDFRecord &data, double &pdf) const final;

        double pdf(const BSDFRecord &data) const final;

        Vector3 f(const BSDFRecord &data) const final;
    };

    using shared_bsdf = std::shared_ptr<BSDF>;


}


#endif //bsdf_HH
