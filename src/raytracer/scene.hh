#ifndef SCENE_HH
#define SCENE_HH
#include "lights/light.hh"

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

        shared_texture get_texture(unsigned obj_id) const;

        Ray init_ray(float x, float y);
        bool find_intersection(const Ray& ray, HitRecord &hit_point) const;

        Vector3 get_color(const Ray& ray, int niter );

        ~Scene(){};
        std::vector<shared_obj> objects;
        std::vector<shared_light> lights;

        int width;
        int height;
        Camera cam;
    private:
        int max_niter;
        Vector3 mid;
    };
}

#endif /* SCENE_HH */
