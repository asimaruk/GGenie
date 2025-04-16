#include "Mesh.h"
#include <vector>

class CubeMesh: public Mesh {
public:
    CubeMesh();
    std::vector<float> getVertices() override;
    std::vector<unsigned int> getIndexes() override;
};