#include <iomanip>
#include <cmath>
#include "raytracer.hh"

namespace raytracing {
    //CAMERA
    Camera::Camera(Vector3 pos, Vector3 forward, Vector3 up, float fov, float aspect_ratio)
        : pos(pos), forward(forward), up(up), fov(fov)
    {
        this->forward.normalize();
        this->up.normalize();

        this->right = forward.cross_product(up);
        this->right.normalize();
        this->right *= aspect_ratio;
    };

    std::ostream& operator<<(std::ostream& out, Camera const& cam){
        return out <<  std::setprecision(2) << "camera : pos" << cam.pos
                   << ", F" << cam.forward << ", U" << cam.up
                   << ", R" << cam.right << ", fov " << cam.fov;
    }

    //SCENE
    Scene::Scene(int width_, int height_, Camera cam_, int max_niter_)
        : width(width_), height(height_),
          cam(cam_), max_niter(max_niter_)
    {
        this->mid = cam.pos + cam.forward / float(tan(cam.fov * M_PI/360));
    };

    void Scene::add_object(std::shared_ptr<Object> obj) {
        this->objects.push_back(obj);
    }

    void Scene::add_light(std::shared_ptr<Light> l) {
        lights.push_back(l);

    }
    Ray Scene::init_ray(float x, float y) {
        float u = (float(height) - 2 * y) / float(height);
        float r = (2 * x - float(width)) / float(width);
        Vector3 pos = mid + cam.up * u + cam.right * r;
        Vector3 direction = pos - cam.pos;
        return Ray(cam.pos, direction);
    }
    std::shared_ptr<Texture> Scene::get_texture(unsigned obj_id) const
    {
        return this->objects[obj_id]->get_texture();
    }

    bool Scene::find_intersection(const Ray& ray, HitRecord& hit_point) const {
        bool hit = false;
        HitRecord tem;
        float min_distance = INFINITY;
        float distance = 0;
        for (unsigned int i=0; i< this->objects.size(); i++)
            if (this->objects[i]->hit(ray, tem)) {
                distance = (tem.point - ray.get_origin()).norm();
                if (!hit || distance < min_distance) {
                    hit_point = tem;
                    hit_point.direction = tem.point - ray.get_origin();
                    hit_point.object_id = i;
                    min_distance = distance;
                }
                hit = true;
            }

        return hit;
    }

    Vector3 Scene::get_color(const Ray& ray, int niter){
        Vector3 color;
        HitRecord hit_data;
        if (this->find_intersection(ray, hit_data))
        {
            shared_texture t = this->get_texture(hit_data.object_id);
            for (auto light: this->lights)
            {
                float intensity =  light->get_intensity(hit_data.point, this->objects);
                color += light->get_color(t, hit_data) * intensity;
            }
            float reflectivity = t->get_reflectivity(hit_data.point);
            if (reflectivity > 0 && niter < this->max_niter)
            {
                Ray r(hit_data.point, ray.get_direction().reflect(hit_data.normal));
                color += this->get_color(r, niter + 1) * reflectivity;

            }
        }
        return color;
    }
}
