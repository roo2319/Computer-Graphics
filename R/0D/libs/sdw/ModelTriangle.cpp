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
  isBump = 0;
  nameTexture = "";
  nameBump = "";


  colour = trigColour;
  normal = glm::normalize(glm::cross(vertices[0] - vertices[1] ,
                           vertices[2] - vertices[0]));
}

Model::Model(std::vector<ModelTriangle> f){
  ofaces = f;
  faces = f;
  velocity = glm::vec3(0);
}

Model::Model(std::vector<ModelTriangle> f, glm::vec3 shift){
  for(unsigned int i = 0; i < f.size(); i++){
    for(int j = 0; j < 3;j++){
      f[i].vertices[j] += shift;
    }
    ofaces.push_back(f[i]);
    faces = ofaces;
  }
}

Model::Model(std::vector<ModelTriangle> f, glm::vec3 s, glm::vec3 v){
  ofaces = f;
  faces = f;
  shift = s;
  velocity = v;

}


void Model::transform(glm::vec3 s,float X, float Y, float Z){
  shift += s;
  glm::mat3 xrot = glm::mat3(1,0,0,0,cos(X),sin(X),0,-sin(X),cos(X));
  glm::mat3 yrot = glm::mat3(cos(Y),0,-sin(Y),0,1,0,sin(Y),0,cos(Y));
  glm::mat3 zrot = glm::mat3(cos(Z),sin(Z),0,-sin(Z),cos(Z),0,0,0,1);
  rotation = xrot * yrot * zrot * rotation;
  for(unsigned int i = 0; i < faces.size(); i++){
    for(int j = 0; j < 3;j++){
      faces[i].vertices[j] = (rotation * ofaces[i].vertices[j]) + shift;
    }
  }
}

void Model::update(bool now){
  if (velocity!=glm::vec3(0)){
    shift+=velocity;
    if (now){
      for(unsigned int i = 0; i < faces.size(); i++){
        for(int j = 0; j < 3;j++){
          faces[i].vertices[j] += velocity;
        }
      }
    }
  }
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
