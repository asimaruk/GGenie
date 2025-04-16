#pragma once

#include <vector>

class Mesh {
protected:
    std::vector<float> vertices = {};
    std::vector<unsigned int> indexes = {};
public:
    virtual std::vector<float> getVertices() = 0;
    virtual std::vector<unsigned int> getIndexes() = 0;
};