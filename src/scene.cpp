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
                   << ", F" << cam.forward << ", U" << cam.up
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
                if (distance < min_distance && distance > 0.0001)
                {
                    hit_point = tem;
                    hit_point.obj_id = i;
                    min_distance = distance;
                }
            }

        return min_distance < INFINITY;
    }

    Vector3 Scene::get_radiance(const Ray& ray, int niter){
        HitRecord hit_data;
        if (!this->find_intersection(ray, hit_data))
            return Vector3(0,0,0);

        if (debug && debug_ray)
            std::cout << ray << "GET_RAD: obj_id: "<< hit_data.obj_id << "\n";

        if (hit_data.normal.dot(ray.get_direction()) > 0)
            hit_data.normal *= -1;

        shared_obj obj = objects[hit_data.obj_id];
        shared_mat mat = obj->material;
        Vector3 obj_c = obj->texture->get_color(hit_data.point);
        Vector3 rad = this->ambient_light * mat->ka;
        if (niter == 0) //avoid double count light source
        {
            rad +=  obj->emitted_rad;

        }
        //BIDIRECTIONAL PATHTRACING
        //LOOP over all objects which is light
        for(unsigned i = 0; i < objects.size(); ++i)
        {
            //To be a light, an object must have emissive radiance
            if (objects[i]->emitted_rad.max() > 0)
            {
                //Ray from hit point to light
                Vector3 dir = objects[i]->get_sample() - hit_data.point;
                Ray sray(hit_data.point + dir * 0.0001, dir);
                if (debug && debug_ray)
                    std::cout << "Detect light object"<< sray << "\n";

                HitRecord tem;
                //Check if sray is not a shadow ray
                if (find_intersection(sray, tem) &&
                    tem.obj_id == i)
                {
                    rad += objects[i]->get_emitted_at(mat, obj->texture,
                                                      hit_data, hit_data.point - cam.pos,
                                                      dir);
                    if (debug && debug_ray)
                    {
                        std::cout << "Not a shadow ray:" << rad << "\n";
                    }
                }
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
        //3 types of reflectance: DIFFUSE, SPECULAR, GLOSSY
        double pd = (mat->kd.x + mat->kd.y + mat->kd.z)/3;
        double ps = (mat->ks.x + mat->ks.y + mat->ks.z)/3;
        double rnd = rand1();

        double k = ps / (pd + ps);
        if (rnd < pd) //DIFFUSE REFLECTANCE
        {
            Vector3 dir = sample_diffuse(hit_data.normal);
            Ray r_ray(hit_data.point, dir);
            rad += get_radiance(r_ray, niter + 1) * obj_c * mat->kd;// * (rnd - k) / (1 - k);
        }
        else if (rnd < pd + ps) //SPECULAR/GLOSSY REFLECTANCE
        {
            //specular:
            //Vector3 dir = ray.get_direction().reflect(hit_data.normal);
            //specular + glossy:
            Vector3 dir = sample_specular(hit_data.normal, mat->ns);
            Ray reflected_ray(hit_data.point, dir);
            rad += get_radiance(reflected_ray, niter + 1) * obj_c * mat->ks;// * rnd/k;
        }
        //TODO: transmit
        return rad;
    }

}
