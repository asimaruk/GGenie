#include "Mesh.h"
#include <span>

class CubeMesh: public Mesh {
public:
    CubeMesh();
    std::span<const float> getVertices() const noexcept override;
    std::span<const unsigned int> getIndices() const noexcept override;
};