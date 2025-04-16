#include "CubeMesh.h"
#include <vector>

CubeMesh::CubeMesh() {
    vertices = {
        // Позиции          // Цвета
        -0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  0.0f, 0.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  0.5f, 0.5f, 0.5f,
        -0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 1.0f
    };
    indexes = {
        0, 1, 2,  0, 2, 3,  // задняя грань
        4, 5, 6,  4, 6, 7,  // передняя грань
        0, 4, 7,  0, 7, 3,  // левая грань
        1, 5, 6,  1, 6, 2,  // правая грань
        3, 2, 6,  3, 6, 7,  // верхняя грань
        0, 1, 5,  0, 5, 4   // нижняя грань
    };
}

std::vector<float> CubeMesh::getVertices() {
    return vertices;
}

std::vector<unsigned int> CubeMesh::getIndexes() {
    return indexes;
}