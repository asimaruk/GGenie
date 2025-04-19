#pragma once

#include <vector>
#include <span>

class Mesh {
protected:
    const std::vector<float> vertices = {};
    const std::vector<unsigned int> indices = {};
    Mesh(std::vector<float>&& v, std::vector<unsigned int>&& i)
        : vertices(std::move(v)), indices(std::move(i)) {}
public:
    std::span<const float> getVertices() const noexcept {
        return vertices;
    }

    std::span<const unsigned int> getIndices() const noexcept {
        return indices;
    }

    virtual ~Mesh() = default;
};