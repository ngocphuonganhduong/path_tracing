#ifndef TEXTURE_HH
#define TEXTURE_HH

#include "../utils/vector3.hh"
#include <memory>

namespace pathtracing {
    //TEXTURE
    class Texture {
    public:
        virtual Vector3 get_color(const Vector3 &point) const = 0;
    };

    class UniformTexture : public Texture {
    public:
        UniformTexture(const Vector3& color);
        Vector3 get_color(const Vector3 &point) const final;

    protected:
        Vector3 color;
    };

    class CheckedTexture : public Texture {
    public:
        CheckedTexture(const Vector3& odd_color, const Vector3& even_color);
        Vector3 get_color(const Vector3 &point) const final;
    private:
        Vector3 odd;
        Vector3 even;
    };

    using shared_text = std::shared_ptr<Texture>;
}
#endif /* TEXTURE_HH */
