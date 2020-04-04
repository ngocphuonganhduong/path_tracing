# include <fstream>
# include <sstream>
# include <string>

# include "object_loader.hh"

namespace pathtracing
{
    namespace objl {
        /* We suppose that a .obj file describes only one object of order 3.
           => load_object() creates a Triangle Mesh. 
       */
        int Loader::load_object(std::string filename, TriVector& t)
        {
            std::ifstream file(filename);

            if (!file.is_open())
            {
                std::cerr << "ERROR: cannot open " << filename << " file.\n";
                return -1;
            }

            std::vector<Vector3> Vertices;
            std::vector<Face> Faces;
            
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
                    Faces.push_back(Face({i1 - 1 , i2 - 1, i3 - 1}));
                }
            }

            for (auto i = Faces.begin(); i != Faces.end(); ++i)
            {
                t.push_back(Triangle({ Vertices[(*i).index[0]],
                                       Vertices[(*i).index[1]],
                                       Vertices[(*i).index[2]] }));
            }
            return 1;
        }
    }
}
