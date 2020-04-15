#pragma once

#include <glm/glm.hpp>
#include <iostream>

class Camera
{
  public:
    glm::vec3 position;
    glm::mat3 rotation;
    float focal;
  
  Camera();
  Camera(glm::vec3 p, glm::mat3 r, float f);
  void updateRotation(float X, float Y, float Z);
  void lookat(glm::vec3 point);
  void forward(float amount);
  void right(float amount);
  void up(float amount);

};
