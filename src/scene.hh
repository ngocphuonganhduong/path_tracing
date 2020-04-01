#ifndef SCENE_HH
#define SCENE_HH

#include "objects/object.hh"

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
        Scene(int width, int height, Camera cam, const Vector3& ambient);
        void add_object(shared_obj obj);

        Ray init_ray(double x, double y);
        bool find_intersection(const Ray& ray, HitRecord &hit_point) const;

        ~Scene(){};
        std::vector<shared_obj> objects; //including objs
        std::vector<shared_obj> lights;

        int width;
        int height;
        Camera cam;
        Vector3 ambient_light;
    private:
        Vector3 mid;
    };
}

#endif /* SCENE_HH */
