#include <iomanip>
#include "render.hh"

namespace pathtracing {
    //CAMERA
    Camera::Camera(Vector3 pos, Vector3 forward, Vector3 up, float fov)
            : pos(pos), forward(forward), up(up), fov(fov) {
        this->forward.normalize();
        this->up.normalize();

        this->right = forward.cross(up);
        this->right.normalize();
    };

    std::ostream &operator<<(std::ostream &out, Camera const &cam) {
        return out << std::setprecision(2) << "camera : pos" << cam.pos
                   << ", F" << cam.forward << ", U" << cam.up
                   << ", R" << cam.right << ", fov " << cam.fov;
    }

    //SCENE
    Scene::Scene(int width_, int height_, Camera cam_, const Vector3 &ambient)
            : width(width_), height(height_), cam(cam_), ambient_light(ambient) {
        cam.right *= float(width) / float(height);
        this->mid = cam.pos + cam.forward / float(tan(cam.fov * M_PI / 360));
    };

    void Scene::add_object(std::shared_ptr <Object> obj) {
        objects.push_back(obj);

        if (obj->material->ke.max() > 0)
            lights.push_back(obj);
    }

    Ray Scene::init_ray(double x, double y) {
        float u = (float(height) - 2 * y) / float(height);
        float r = (2 * x - float(width)) / float(width);
        Vector3 pos = mid + cam.up * u + cam.right * r;
        if (debug && debug_ray)
            std::cout << pos << " - " << cam.pos << "\n";
        Vector3 direction = pos - cam.pos; //cam to pos
        return Ray(cam.pos, direction);
    }

    bool Scene::find_intersection(const Ray &ray, HitRecord &hit_point) const {
        HitRecord tem;
        float min_distance = INFINITY;
        float distance = 0;
        for (unsigned int i = 0; i < this->objects.size(); i++)
            if (this->objects[i]->hit(ray, tem)) {
                distance = (tem.point - ray.get_origin()).norm();
                if (distance < min_distance && distance > 0.0001) {
                    hit_point = tem;
                    hit_point.obj_id = i;
                    min_distance = distance;
                }
            }
        hit_point.normal.normalize();
        return min_distance < INFINITY;
    }
}
