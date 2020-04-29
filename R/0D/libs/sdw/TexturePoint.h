#pragma once

#include <iostream>

class TexturePoint
{
  public:
    float x;
    float y;

    TexturePoint();

    TexturePoint(float xPos, float yPos);


    TexturePoint operator-(const TexturePoint& t);

    TexturePoint operator+(const TexturePoint& t);

    TexturePoint operator/(const float f);
};

std::ostream& operator<<(std::ostream& os, const TexturePoint& point);
TexturePoint operator*(float f, TexturePoint t);