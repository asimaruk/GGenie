#pragma once

#include <vector>
#include <span>

class Mesh {
private:
    const std::vector<float> vertices = {};
    const std::vector<unsigned int> indices = {};
public:
    Mesh(std::vector<float>&& v, std::vector<unsigned int>&& i);
    virtual std::span<const float> getVertices() const noexcept final;
    virtual std::span<const unsigned int> getIndices() const noexcept final;
    virtual ~Mesh() = default;
};