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
    bool isBump;
    std::string nameBump;
    std::string nameTexture;
    std::vector<std::vector<glm::vec3>>* bump;
    std::vector<std::vector<uint32_t>> image;
    //for performance make it just accept a packed colour or pack in the constructors

    ModelTriangle();
    ModelTriangle(glm::vec3 v0, glm::vec3 v1, glm::vec3 v2, Colour trigColour);

};

class Model
{
  public:
    std::vector<ModelTriangle> ofaces;
    std::vector<ModelTriangle> faces;

    glm::vec3 shift;
    glm::mat3 rotation;

    glm::vec3 rockstart;
    glm::vec3 rockdir;
    //for performance make it just accept a packed colour or pack in the constructors

    Model();
    Model(std::vector<ModelTriangle> faces);
    Model(std::vector<ModelTriangle> faces, glm::vec3 shift);


    void transform(glm::vec3 dist,float X, float Y, float Z);
    void texture(std::vector<std::vector<uint32_t>> image);
    void rockUpdate(int cameraDepth);
};

std::ostream& operator<<(std::ostream& os, const ModelTriangle& triangle);
bool operator==(ModelTriangle a, ModelTriangle b);
bool operator!=(ModelTriangle a, ModelTriangle b);
