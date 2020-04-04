# pragma once

# include <string>

# include "object.hh"
# include "../utils/vector3.hh"

namespace pathtracing
{
    namespace objl {

        typedef struct { unsigned int index[3]; } Face;

        class Loader
        {
        public:
            Loader()
            {}

            int load_object(std::string filename, TriVector& t);
        };

    }
}
