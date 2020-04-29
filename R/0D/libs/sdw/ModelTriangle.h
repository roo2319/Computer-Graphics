#pragma once

#include "glm/glm.hpp"
#include "Colour.h"
#include "TexturePoint.h"
#include <vector>
#include <string>

class ModelTriangle
{
  public:
    glm::vec3 vertices[3];
    glm::vec3 normal;
    Colour colour;
    TexturePoint texture[3];
    bool isTexture;
    bool isSpecular;
    std::string nameTexture;
    std::vector<vector<uint32_t>> image;
    //for performance make it just accept a packed colour or pack in the constructors

    ModelTriangle();
    ModelTriangle(glm::vec3 v0, glm::vec3 v1, glm::vec3 v2, Colour trigColour);

};

class Model
{
  public:
    std::vector<ModelTriangle> faces;
    std::vector<vector<uint32_t>> image;
    //for performance make it just accept a packed colour or pack in the constructors

    Model();
    Model(std::vector<ModelTriangle> f);
    Model(std::vector<ModelTriangle> f, vector<vector<uint32_t>> i);
    void Model::shift( glm::vec3 dist);
    void Model::rotate(float X, float Y, float Z);
};

std::ostream& operator<<(std::ostream& os, const ModelTriangle& triangle);
bool operator==(ModelTriangle a, ModelTriangle b);
bool operator!=(ModelTriangle a, ModelTriangle b);
