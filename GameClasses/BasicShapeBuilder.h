//
// Created by drive on 4/7/21.
//

#ifndef OPENGLTEST_BASICSHAPEBUILDER_H
#define OPENGLTEST_BASICSHAPEBUILDER_H
#include <iostream>
#include <vector>
#include "Mesh.h"
struct Quad {
    float vertices[32] = {
            -0.5, -0.5, 0.0,   0.0, 1.0, 0.0,   0.0, 0.0,
            -0.5, 0.5, 0.0,   0.0, 1.0, 0.0,   0.0, 1.0,
            0.5, -0.5,-0.0,   0.0, 1.0, 0.0,   1.0, 0.0,
            0.5, 0.5,0.0,   0.0, 1.0, 0.0,   1.0, 1.0
    };
    unsigned int indices[6] = {
            0, 1, 2, 1, 3, 2
    };
};
struct Cube {
    float vertices[192] = {
            -0.5, -0.5,  0.5,   0.0,  0.0,  1.0,   0.0, 0.0,
            0.5, -0.5,  0.5,   0.0,  0.0,  1.0,   1.0, 0.0,
            0.5,  0.5,  0.5,   0.0,  0.0,  1.0,   1.0, 1.0,
            -0.5,  0.5,  0.5,   0.0,  0.0,  1.0,   0.0, 1.0,
            0.5, -0.5,  0.5,   1.0,  0.0,  0.0,   0.0, 0.0,
            0.5, -0.5, -0.5,   1.0,  0.0,  0.0,   1.0, 0.0,
            0.5,  0.5, -0.5,   1.0,  0.0,  0.0,   1.0, 1.0,
            0.5,  0.5,  0.5,   1.0,  0.0,  0.0,   0.0, 1.0,
            0.5, -0.5, -0.5,   0.0,  0.0, -1.0,   0.0, 0.0,
            -0.5, -0.5, -0.5,   0.0,  0.0, -1.0,   1.0, 0.0,
            -0.5,  0.5, -0.5,   0.0,  0.0, -1.0,   1.0, 1.0,
            0.5,  0.5, -0.5,   0.0,  0.0, -1.0,   0.0, 1.0,
            -0.5, -0.5, -0.5,  -1.0,  0.0,  0.0,   0.0, 0.0,
            -0.5, -0.5,  0.5,  -1.0,  0.0,  0.0,   1.0, 0.0,
            -0.5,  0.5,  0.5,  -1.0,  0.0,  0.0,   1.0, 1.0,
            -0.5,  0.5, -0.5,  -1.0,  0.0,  0.0,   0.0, 1.0,
            -0.5,  0.5,  0.5,   0.0,  1.0,  0.0,   0.0, 0.0,
            0.5,  0.5,  0.5,   0.0,  1.0,  0.0,   1.0, 0.0,
            0.5,  0.5, -0.5,   0.0,  1.0,  0.0,   1.0, 1.0,
            -0.5,  0.5, -0.5,   0.0,  1.0,  0.0,   0.0, 1.0,
            -0.5, -0.5, -0.5,   0.0, -1.0,  0.0,   0.0, 0.0,
            0.5, -0.5, -0.5,   0.0, -1.0,  0.0,   1.0, 0.0,
            0.5, -0.5,  0.5,   0.0, -1.0,  0.0,   1.0, 1.0,
            -0.5, -0.5,  0.5,   0.0, -1.0,  0.0,   0.0, 1.0
    };
    unsigned int indices[36] = {
        0,  1,  2,  0,  2,  3,
        4,  5,  6,  4,  6,  7,
        8,  9, 10,  8, 10, 11,
        12, 13, 14, 12, 14, 15,
        16, 17, 18, 16, 18, 19,
        20, 21, 22, 20, 22, 23
    };

};

struct Cover {
    float vertices[104] = {
            1,      0,     0.0,   0.0,  0.0,  1.0,   1,     0.5,
            0.866,  0.5,   0.0,   0.0,  0.0,  1.0,   0.933, 0.75,
            0.5,    0.866, 0.0,   0.0,  0.0,  1.0,   0.75,  0.933,
            0,      1,     0.0,   0.0,  0.0,  1.0,   0.5,   1,
            -0.5,    0.866, 0.0,   0.0,  0.0,  1.0,   0.25,  0.933,
            -0.86,   0.5,   0.0,   0.0,  0.0,  1.0,   0.07,  0.75,
            -1,      0,     0.0,   0.0,  0.0,  1.0,   0,     0.5,
            -0.866, -0.5,   0.0,   0.0,  0.0,  1.0,   0.067, 0.25,
            -0.5,   -0.866, 0.0,   0.0,  0.0,  1.0,   0.25,  0.067,
            0,     -1,     0.0,   0.0,  0.0,  1.0,   0.5,   0,
            0.5,   -0.866, 0.0,   0.0,  0.0,  1.0,   0.75,  0.067,
            0.866, -0.5,   0.0,   0.0,  0.0,  1.0,   0.933, 0.25,
            0,      0,     0.0,   0.0,  0.0,  1.0,   0.5,   0.5
    };
    unsigned int indices[36] = {
            0, 1, 12, 1, 2, 12, 2, 3, 12, 3,  4, 12,  4,  5, 12,  5, 6, 12,
            6, 7, 12, 7, 8, 12, 8, 9, 12, 9, 10, 12, 10, 11, 12, 11, 0, 12
    };
};
struct Cone {
    float vertices[192] = {
            1.000,  0.000, 0.000,  0.707,  0.000, 0.707, 1.00, 0.50,
            0.000,  0.000, 1.000,  0.683,  0.183, 0.707, 0.50, 0.50,
            0.866,  0.500, 0.000,  0.612,  0.354, 0.707, 0.93, 0.75,
            0.000,  0.000, 1.000,  0.500,  0.500, 0.707, 0.50, 0.50,
            0.500,  0.866, 0.000,  0.354,  0.612, 0.707, 0.75, 0.93,
            0.000,  0.000, 1.000,  0.183,  0.683, 0.707, 0.50, 0.50,
            0.000,  1.000, 0.000,  0.000,  0.707, 0.707, 0.50, 1.00,
            0.000,  0.000, 1.000, -0.183,  0.683, 0.707, 0.50, 0.50,
            -0.500,  0.866, 0.000, -0.354,  0.612, 0.707, 0.25, 0.93,
            0.000,  0.000, 1.000, -0.500,  0.500, 0.707, 0.50, 0.50,
            -0.866,  0.500, 0.000, -0.612,  0.354, 0.707, 0.07, 0.75,
            0.000,  0.000, 1.000, -0.683,  0.183, 0.707, 0.50, 0.50,
            -1.000,  0.000, 0.000, -0.707,  0.000, 0.707, 0.00, 0.50,
            0.000,  0.000, 1.000, -0.683, -0.183, 0.707, 0.50, 0.50,
            -0.866, -0.500, 0.000, -0.612, -0.354, 0.707, 0.07, 0.25,
            0.000,  0.000, 1.000, -0.500, -0.500, 0.707, 0.50, 0.50,
            -0.500, -0.866, 0.000, -0.354, -0.612, 0.707, 0.25, 0.07,
            0.000,  0.000, 1.000, -0.183, -0.683, 0.707, 0.50, 0.50,
            0.000, -1.000, 0.000, -0.000, -0.707, 0.707, 0.50, 0.00,
            0.000,  0.000, 1.000,  0.183, -0.683, 0.707, 0.50, 0.50,
            0.500, -0.866, 0.000,  0.354, -0.612, 0.707, 0.75, 0.07,
            0.000,  0.000, 1.000,  0.500, -0.500, 0.707, 0.50, 0.50,
            0.866, -0.500, 0.000,  0.612, -0.354, 0.707, 0.93, 0.25,
            0.000,  0.000, 1.000,  0.683, -0.183, 0.707, 0.50, 0.50
    };
    unsigned int indices[36] = {
            0,  2,  1,  2,  4,  3,  4,  6,  5,  6,  8,  7,  8, 10,  9, 10, 12, 11,
            12, 14, 13, 14, 16, 15, 16, 18, 17, 18, 20, 19, 20, 22, 21, 22,  0, 23
    };
};
struct Cylinder {
    float vertices[208] = {
            1.000,  0.000, 0.000,  1.000,  0.000, 0.000, 0.000, 0.000,
            0.866,  0.500, 0.000,  0.866,  0.500, 0.000, 0.083, 0.000,
            0.500,  0.866, 0.000,  0.500,  0.866, 0.000, 0.167, 0.000,
            0.000,  1.000, 0.000,  0.000,  1.000, 0.000, 0.250, 0.000,
            -0.500,  0.866, 0.000, -0.500,  0.866, 0.000, 0.333, 0.000,
            -0.860,  0.500, 0.000, -0.860,  0.500, 0.000, 0.417, 0.000,
            -1.000,  0.000, 0.000, -1.000,  0.000, 0.000, 0.500, 0.000,
            -0.866, -0.500, 0.000, -0.866, -0.500, 0.000, 0.583, 0.000,
            -0.500, -0.866, 0.000, -0.500, -0.866, 0.000, 0.667, 0.000,
            0.000, -1.000, 0.000,  0.000, -1.000, 0.000, 0.750, 0.000,
            0.500, -0.866, 0.000,  0.500, -0.866, 0.000, 0.833, 0.000,
            0.866, -0.500, 0.000,  0.866, -0.500, 0.000, 0.917, 0.000,
            1.000,  0.000, 0.000,  1.000,  0.000, 0.000, 1.000, 0.000,
            1.000,  0.000, 1.000,  1.000,  0.000, 0.000, 0.000, 1.000,
            0.866,  0.500, 1.000,  0.866,  0.500, 0.000, 0.083, 1.000,
            0.500,  0.866, 1.000,  0.500,  0.866, 0.000, 0.167, 1.000,
            0.000,  1.000, 1.000,  0.000,  1.000, 0.000, 0.250, 1.000,
            -0.500,  0.866, 1.000, -0.500,  0.866, 0.000, 0.333, 1.000,
            -0.860,  0.500, 1.000, -0.860,  0.500, 0.000, 0.417, 1.000,
            -1.000,  0.000, 1.000, -1.000,  0.000, 0.000, 0.500, 1.000,
            -0.866, -0.500, 1.000, -0.866, -0.500, 0.000, 0.583, 1.000,
            -0.500, -0.866, 1.000, -0.500, -0.866, 0.000, 0.667, 1.000,
            0.000, -1.000, 1.000,  0.000, -1.000, 0.000, 0.750, 1.000,
            0.500, -0.866, 1.000,  0.500, -0.866, 0.000, 0.833, 1.000,
            0.866, -0.500, 1.000,  0.866, -0.500, 0.000, 0.917, 1.000,
            1.000,  0.000, 1.000,  1.000,  0.000, 0.000, 1.000, 1.000
    };
    unsigned int indices[72] = {
            0,  1, 13, 1,  2, 14, 2,  3, 15,  3,  4, 16,  4,  5, 17,  5,  6, 18,
            6,  7, 19, 7,  8, 20, 8,  9, 21,  9, 10, 22, 10, 11, 23, 11, 12, 24,
            1, 14, 13, 2, 15, 14, 3, 16, 15,  4, 17, 16,  5, 18, 17,  6, 19, 18,
            7, 20, 19, 8, 21, 20, 9, 22, 21, 10, 23, 22, 11, 24, 23, 12, 25, 24
    };
};
static Mesh procesMesh(float vertices[], unsigned int indices[],const char * path, std::string dir, int nVertices, int nIndices)
{
    std::vector<Vertex> vertexes;
    std::vector<Texture> text;
    std::vector<unsigned int > indexes(indices,indices+nIndices);
    for (int i = 0; i < nVertices; i+=8)
    {
        Vertex actualVertex;
        actualVertex.Position = glm::vec3(vertices[i],vertices[i+1],vertices[i+2]);
        actualVertex.Normal = glm::vec3(vertices[i+3],vertices[i+4],vertices[i+5]);
        actualVertex.TexCoords = glm::vec2(vertices[i+6],vertices[i+7]);
        vertexes.push_back(actualVertex);
    }
    Mesh res(vertexes, indexes, path, dir);
    return res;
}

#endif //OPENGLTEST_BASICSHAPEBUILDER_H
