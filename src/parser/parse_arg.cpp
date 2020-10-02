#include "parser.hh"
#include <cstring>

namespace pathtracing {

    void parse_arguments(Pathtracer& pt, int argc, char **argv, double& x, double& y)
    {

        for (int i = 2; i < argc; ++i)
        {
            if (strcmp(argv[i], "--debug") == 0 || strcmp(argv[i], "-d") == 0) {
                debug = true;
            }
            else if (strcmp(argv[i], "-ns") == 0){
                ++i;
                pt.set_nb_samples(std::atoi(argv[i]));
            }
            else if (debug && (strcmp(argv[i], "--ray") == 0
                               || strcmp(argv[i], "-r") == 0))
            {
                debug_ray = true;
                x = std::atof(argv[i + 1]);
                y = std::atof(argv[i + 2]);
                i += 2;
            }
            else if (strcmp(argv[i], "--indirect") == 0) //nb of indirect bounces
            {
                i += 1;
                pt.max_idl_bounce = std::atoi(argv[i]);
            }
            else if (strcmp(argv[i], "--direct") == 0) //nb of indirect bounces
            {
                i += 1;
                pt.max_dl_bounce = std::atoi(argv[i]);
            }
            else if (strcmp(argv[i], "--filename") == 0 ||
                     strcmp(argv[i], "-fn") == 0) //nb of indirect bounces
            {
                i += 1;
                pt.filename = argv[i];
            }
            else if (strcmp(argv[i], "--classic") == 0)
            {
                pt.mode = CLASSIC_PT;
            }
            else if (strcmp(argv[i], "--bdpt") == 0)
            {
                pt.mode = BDPT;
            }

        }
    }

}
