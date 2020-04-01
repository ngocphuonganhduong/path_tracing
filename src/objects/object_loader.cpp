# include <fstream>
# include <sstream>
# include <string>

# include "object_loader.hh"

namespace pathtracing
{
    namespace objl {
        
        int Loader::load(std::string filename)
        {
            std::ifstream file(filename);

            if (!file.is_open())
            {
                std::cerr << "ERROR: cannot open " << filename << " file.\n";
                return -1;
            }

            std::string currline;

            while (std::getline(file, currline))
            {
                if (currline.empty())
                    continue;

                std::istringstream iss(currline);

                std::string token;
                iss >> token;

                if (token.compare("#") == 0)
                    continue;

                if (token.compare("v") == 0)
                {
                    float a, b, c;
                    iss >> a >> b >> c;
                    Vertices.push_back(Vector3(a, b, c));
                }

                if (token.compare("f") == 0)
                {
                    unsigned int i1, i2, i3;
                    iss >> i1 >> i2 >> i3;
                    Faces.push_back(Face({i1, i2, i3}));
                }

                if (token.compare("g") == 0)
                {
                    // FIXME : Define an object -> create the triangle mesh before the other "g" if there is any
                }
                
                // FIXME : Create objects -> a face is a triangle, an object a triangle mesh
            }

            return 1;
        }
    }
}
