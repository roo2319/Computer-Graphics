#pragma once

#include <iostream>
#include <glm/glm.hpp>

class TexturePoint
{
  public:
    float x;
    float y;
    glm::vec2 point;

    TexturePoint();

    TexturePoint(float xPos, float yPos);
    TexturePoint(glm::vec2 p);


    TexturePoint operator-(const TexturePoint& t);

    TexturePoint operator+(const TexturePoint& t);
    bool operator==(const TexturePoint& t);

    TexturePoint operator/(const float f);
};

std::ostream& operator<<(std::ostream& os, const TexturePoint& point);

TexturePoint operator*(float f, TexturePoint t);