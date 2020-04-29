#include "ModelTriangle.h"

ModelTriangle::ModelTriangle()
{
}

ModelTriangle::ModelTriangle(glm::vec3 v0, glm::vec3 v1, glm::vec3 v2, Colour trigColour)
{
  vertices[0] = v0;
  vertices[1] = v1;
  vertices[2] = v2;

  texture[0]  = TexturePoint(-1,-1);
  texture[1]  = TexturePoint(-1,-1);
  texture[2]  = TexturePoint(-1,-1);
  isTexture = 0;
  nameTexture = "";
  
  colour = trigColour;
  normal = glm::normalize(glm::cross(vertices[0] - vertices[1] ,
                           vertices[2] - vertices[0]));
}

Model::Model(std::vector<ModelTriangle> f){
  faces = f;
}

Model::Model(std::vector<ModelTriangle> f, glm::vec3 shift){
  for(uint i = 0; i < f.size(); i++){
    for(uint j = 0; j < 3;j++){
      f[i].vertices[j] += shift;
    }
    faces.push_back(f[i]);
  }
}

void Model::update(){
  ;
  // std::cout << "Boring Update" << std::endl;
}


std::ostream& operator<<(std::ostream& os, const ModelTriangle& triangle)
{
    os << "(" << triangle.vertices[0].x << ", " << triangle.vertices[0].y << ", " << triangle.vertices[0].z << ")" << std::endl;
    os << "(" << triangle.vertices[1].x << ", " << triangle.vertices[1].y << ", " << triangle.vertices[1].z << ")" << std::endl;
    os << "(" << triangle.vertices[2].x << ", " << triangle.vertices[2].y << ", " << triangle.vertices[2].z << ")" << std::endl;
    os << std::endl;
    return os;
}

bool operator==(ModelTriangle a, ModelTriangle b){
  return (a.vertices[0] == b.vertices[0])
      && (a.vertices[1] == b.vertices[1])
      && (a.vertices[2] == b.vertices[2]);
}

bool operator!=(ModelTriangle a, ModelTriangle b){
  return !(a==b);
}
