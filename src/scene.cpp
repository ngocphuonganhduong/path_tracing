#include <iomanip>
#include "pathtracer.hh"

namespace pathtracing {
    //CAMERA
    Camera::Camera(Vector3 pos, Vector3 forward, Vector3 up, float fov)
        : pos(pos), forward(forward), up(up), fov(fov)
    {
        this->forward.normalize();
        this->up.normalize();

        this->right = forward.cross(up);
        this->right.normalize();
    };

    std::ostream& operator<<(std::ostream& out, Camera const& cam){
        return out <<  std::setprecision(2) << "camera : pos" << cam.pos
5B5B5B                   << ", F" << cam.forward << ", U" << cam.up
                   << ", R" << cam.right << ", fov " << cam.fov;
    }

    //SCENE
    Scene::Scene(int width_, int height_, Camera cam_, int max_niter_,
                 const Vector3& ambient)
        : width(width_), height(height_), cam(cam_), max_niter(max_niter_),
          ambient_light(ambient)
    {
        cam.right *= float(width)/float(height);
        this->mid = cam.pos + cam.forward / float(tan(cam.fov * M_PI/360));
    };

    void Scene::add_object(std::shared_ptr<Object> obj) {
        this->objects.push_back(obj);
    }

    Ray Scene::init_ray(float x, float y) {
        float u = (float(height) - 2 * y) / float(height);
        float r = (2 * x - float(width)) / float(width);
        Vector3 pos = mid + cam.up * u + cam.right * r;
        Vector3 direction = pos - cam.pos;
        return Ray(cam.pos, direction);
    }

    bool Scene::find_intersection(const Ray& ray, HitRecord& hit_point) const {
        HitRecord tem;
        float min_distance = INFINITY;
        float distance = 0;
        for (unsigned int i=0; i< this->objects.size(); i++)
            if (this->objects[i]->hit(ray, tem))
            {
                distance = (tem.point - ray.get_origin()).norm();
                if (distance < min_distance)
                {
                    hit_point = tem;
//                    hit_point.direction = tem.point - ray.get_origin();
                    hit_point.obj_id = i;
                    min_distance = distance;
                }
            }

        return min_distance != INFINITY;
    }

    Vector3 Scene::get_radiance(const Ray& ray, int niter, bool ignore_light){
        HitRecord hit_data;
        if (!this->find_intersection(ray, hit_data))
            return Vector3(0,0,0);

        Vector3 normal = hit_data.normal;
        if (normal.dot(ray.get_direction()) > 0)
            normal *= -1;

        shared_obj obj = objects[hit_data.obj_id];
        shared_mat mat = obj->material;
        Vector3 obj_c = obj->texture->get_color(hit_data.point);
        Vector3 rad = this->ambient_light * mat->ka;
        if (!ignore_light)
            rad += obj->emitted_rad; //radiance/color

        //BIDIRECTIONAL PATHTRACING
        for(auto o: objects)
        {
            if (o->emitted_rad.max() > 0)
            {
                Vector3 pos = o->get_sample();
                Ray lray(pos, pos - hit_data.point);
                
            }
        }
        if (niter > max_niter)
        {
            //ROUSSIAN ROULETTE to terminate path
            double p = obj_c.max();
            if (rand1() < p)
                obj_c /= p;
            else
                return rad;
        }
        //ROUSSIAN ROULETTE to decide with types of reflectance
        //
        //double max_r = mat->kd + mat->ks;
        //double rnd = max_r * rand1();
        //if (rnd < mat->kd)
        //{
        Vector3 dir = sample_diffuse(normal);
        Ray r_ray(hit_data.point, dir);
        rad += obj_c * get_radiance(r_ray, niter + 1) * mat->kd;// * mat->kd / max_r;
            //}
//        else
            //      {
            //TODO
            //Vector3 dir = sample_specular(hit_data.normal);

            //Ray r_ray(hit_data.point, dir);
            //rad += get_radiance(r_ray, niter + 1) * mat->kd * mat->kd / max_r;
            // }
        return rad;
    }

}
