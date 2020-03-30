#include "parser/parser.hh"
# include "objects/object_loader.hh"

using namespace pathtracing;

int main() {
    
    objl::Loader loader;

    loader.load("src/objects/assets/diamond.obj");
    return 0;
}
