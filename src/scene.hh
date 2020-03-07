#ifndef SCENE_HH
#define SCENE_HH

#include "objects/object.hh"
#include "brdf.hh"

namespace pathtracing {
    //CAMERA
    class Camera {
    public:
        Camera(Vector3 pos, Vector3 forward, Vector3 up, float fov);
        friend std::ostream& operator<<(std::ostream& out, Camera const& cam);
        Vector3 pos;
        Vector3 forward;
        Vector3 up;
        Vector3 right;
        float fov; //field of view
    };


    //SCENE
    class Scene {
    public:
        Scene(int width, int height, Camera cam, int max_niter,
              const Vector3& ambient);
        void add_object(shared_obj obj);

        Ray init_ray(float x, float y);
        bool find_intersection(const Ray& ray, HitRecord &hit_point) const;

        Vector3 get_radiance(const Ray& ray, int niter);
        ~Scene(){};
        std::vector<shared_obj> objects;

        int width;
        int height;
        Camera cam;
    private:
        int max_niter; //max number of bounces
        Vector3 mid;
        Vector3 ambient_light;
    };
}

#endif /* SCENE_HH */
