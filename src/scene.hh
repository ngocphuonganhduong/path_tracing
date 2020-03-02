#ifndef SCENE_HH
#define SCENE_HH
#include "light.hh"
#include "ray.hh"
#include "object.hh"

namespace raytracing {
    //CAMERA
    class Camera {
    public:
        Camera(Vector3 pos, Vector3 forward, Vector3 up, float fov,
               float aspect_ratio);
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
        Scene(int width, int height, Camera cam, int max_niter);
        void add_object(std::shared_ptr<Object> obj);
        void add_light(std::shared_ptr<Light> obj);
        int get_width() const;
        int get_height() const;
        shared_texture get_texture(unsigned obj_id) const;

        Ray init_ray(float x, float y);
        bool find_intersection(const Ray& ray, HitRecord &hit_point) const;
        float is_shadow_ray(const Ray& ray, unsigned  obj_id) const;
        Vector3 get_color(const Ray& ray, int niter );
        ~Scene(){};
        std::vector<shared_obj> objects;
        std::vector<std::shared_ptr<Light>> lights;

    private:
        int width_;
        int height_;
        Camera cam_;
        int max_niter_;
        Vector3 mid_;
    };
}

#endif /* SCENE_HH */
