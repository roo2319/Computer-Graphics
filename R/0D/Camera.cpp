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

void Camera::lookat(glm::vec3 point){
  glm::vec3 forward = -glm::normalize(position-point);
  glm::vec3 right   = glm::cross(glm::vec3(0,1,0),forward);
  glm::vec3 up      = glm::cross(forward,right);
  
  rotation[0][0] = right.x; 
  rotation[1][0] = right.y; 
  rotation[2][0] = right.z; 
  rotation[0][1] = up.x; 
  rotation[1][1] = up.y; 
  rotation[2][1] = up.z; 
  rotation[0][2] = forward.x; 
  rotation[1][2] = forward.y; 
  rotation[2][2] = forward.z; 


}

void Camera::forward(int amount){
  position += glm::normalize(glm::vec3(0,0,amount) * rotation);
}
void Camera::right(int amount){
  position += glm::normalize(glm::vec3(amount,0,0) * rotation);
}
void Camera::up(int amount){
  position += glm::normalize(glm::vec3(0,amount,0) * rotation);
}
