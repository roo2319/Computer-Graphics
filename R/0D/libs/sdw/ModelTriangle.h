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
    std::string nameTexture;
    //for performance make it just accept a packed colour or pack in the constructors

    ModelTriangle();
    ModelTriangle(glm::vec3 v0, glm::vec3 v1, glm::vec3 v2, Colour trigColour);

};

class Model
{
  public:
    std::vector<ModelTriangle> faces;
    //for performance make it just accept a packed colour or pack in the constructors

    Model();
    Model(std::vector<ModelTriangle> faces);

    void update();
};

class RotatableModel : public Model
{ 
  public:
    RotatableModel(std::vector<ModelTriangle> f):Model{f} {}
    void update();
};

std::ostream& operator<<(std::ostream& os, const ModelTriangle& triangle);
bool operator==(ModelTriangle a, ModelTriangle b);
bool operator!=(ModelTriangle a, ModelTriangle b);
