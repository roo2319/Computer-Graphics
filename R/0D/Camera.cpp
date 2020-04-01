#include "Camera.h"

Camera::Camera()
{
}

Camera::Camera(glm::vec3 p, glm::mat3 r, float f)
{
    position = p; 
    rotation = r;
    focal    = f;
}

void Camera::updateRotation(float X, float Y, float Z){
  glm::mat3 xrot = glm::mat3(1,0,0,0,cos(X),sin(X),0,-sin(X),cos(X));
  glm::mat3 yrot = glm::mat3(cos(Y),0,-sin(Y),0,1,0,sin(Y),0,cos(Y));
  glm::mat3 zrot = glm::mat3(cos(Z),sin(Z),0,-sin(Z),cos(Z),0,0,0,1);
  rotation = rotation * xrot * yrot * zrot;
}