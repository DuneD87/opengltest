///////////////////////////////////////////////////////////////////////////////
// Cubesphere.h
// ============
// cube-based sphere dividing the spherical surface into 6 equal-area faces of
// a cube (+X, -X, +Y, -Y, +Z, -Z)
// If N=0, it is identical to a cube, which is inscribed in a sphere.
//
//  AUTHOR: Song Ho Ahn (song.ahn@gmail.com)
// CREATED: 2018-09-20
// UPDATED: 2019-12-28
///////////////////////////////////////////////////////////////////////////////

#ifndef GEOMETRY_CUBESPHERE_H
#define GEOMETRY_CUBESPHERE_H
#include "../../Basic/Mesh.h"
#include "../../Util/FastNoiseLite.h"
#include <vector>
#include <map>
#include <cmath>

class Cubesphere
{
public:
    // ctor/dtor
    Cubesphere(float radius=1.0f, int subdivision=3, bool smooth=true);
    ~Cubesphere() {}
    std::vector<Vertex> vertexList;
    // getters/setters
    float getRadius() const                 { return radius; }
    void setRadius(float radius);
    float getSideLength() const             { return radius * 2 / sqrt(3.0f); }
    void setupNoise(float height, FastNoiseLite noise);

    // for vertex data
    unsigned int getVertexCount() const     { return (unsigned int)vertices.size() / 3; }
    unsigned int getNormalCount() const     { return (unsigned int)normals.size() / 3; }
    unsigned int getTexCoordCount() const   { return (unsigned int)texCoords.size() / 2; }
    unsigned int getIndexCount() const      { return (unsigned int)indices.size(); }
    unsigned int getTriangleCount() const   { return getIndexCount() / 3; }
    std::vector<unsigned int> getIndices() const  { return indices; }
  // draw only single face, valid ID is 0~5

    // debug
    void printSelf() const;

protected:

private:
    // static functions
    static void computeFaceNormal(const float v1[3], const float v2[3], const float v3[3], float normal[3]);
    static float computeScaleForLength(const float v[3], float length);
    static std::vector<float> getUnitPositiveX(unsigned int pointsPerRow);


    // member functions
    void clearArrays();
    void updateRadius();

    void buildVerticesSmooth();
    void buildInterleavedVertices();
    void addVertex(float x, float y, float z);
    void addNormal(float nx, float ny, float nz);
    void addTexCoord(float s, float t);
    void addIndices(unsigned int i1, unsigned int i2, unsigned int i3);
    void computeNormals();

    // memeber vars
    float radius;                           // circumscribed radius
    int subdivision;
    bool smooth;
    std::vector<float> vertices;
    std::vector<float> normals;
    std::vector<float> texCoords;
    std::vector<unsigned int> indices;
    std::vector<unsigned int> lineIndices;
    float lPoint,hPoint;
public:
    float getLPoint() const;

    float getHPoint() const;

private:
    // interleaved
    std::vector<float> interleavedVertices;
    int interleavedStride;                  // # of bytes to hop to the next vertex (should be 32 bytes)
    FastNoiseLite noise;
    float height = 100;
    // for face
    unsigned int vertexCountPerRow;
    unsigned int vertexCountPerFace;
};

#endif
