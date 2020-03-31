#pragma once

#include "TexturePoint.h"
#include <iostream>

class CanvasPoint
{
  public:
    float x;
    float y;
    double depth;
    float brightness;
    TexturePoint texturePoint;

    CanvasPoint();
    CanvasPoint(float xPos, float yPos);

    CanvasPoint(float xPos, float yPos, float pointDepth);

    CanvasPoint(float xPos, float yPos, float pointDepth, float pointBrightness);

    CanvasPoint(float xPos, float yPos, TexturePoint tp);

    //Undefined behaviour to add or subtract -1 texture point
    CanvasPoint operator-(const CanvasPoint& c);

    CanvasPoint operator+(const CanvasPoint& c);

    CanvasPoint operator/(const float f);

};

std::ostream& operator<<(std::ostream& os, const CanvasPoint& point);

CanvasPoint operator*(float f, CanvasPoint c);
