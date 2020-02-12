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

    CanvasPoint()
    {
        texturePoint = TexturePoint(-1,-1);
    }

    CanvasPoint(float xPos, float yPos)
    {
      x = xPos;
      y = yPos;
      depth = 0.0;
      brightness = 1.0;
      texturePoint = TexturePoint(-1,-1);
    }

    CanvasPoint(float xPos, float yPos, float pointDepth)
    {
      x = xPos;
      y = yPos;
      depth = pointDepth;
      brightness = 1.0;
      texturePoint = TexturePoint(-1,-1);
    }

    CanvasPoint(float xPos, float yPos, float pointDepth, float pointBrightness)
    {
      x = xPos;
      y = yPos;
      depth = pointDepth;
      brightness = pointBrightness;
      texturePoint = TexturePoint(-1,-1);
    }

    CanvasPoint(float xPos, float yPos, TexturePoint tp){
      x = xPos;
      y = yPos;
      texturePoint = tp;
    }

    //Undefined behaviour to add or subtract -1 texture point
    CanvasPoint operator-(const CanvasPoint& c){
      return CanvasPoint(this->x-c.x, this->y-c.y, this->texturePoint - c.texturePoint);
    }

    CanvasPoint operator+(const CanvasPoint& c){
      return CanvasPoint(this->x+c.x, this->y+c.y, this->texturePoint + c.texturePoint);
    }

    CanvasPoint operator/(const float f){
      return CanvasPoint(this->x/f,this->y/f,this->texturePoint/f);
    }

};

std::ostream& operator<<(std::ostream& os, const CanvasPoint& point)
{
    os << "(" << point.x << ", " << point.y << ", " << point.depth << ") " << point.brightness << std::endl;
    return os;
}
CanvasPoint operator*(float f, CanvasPoint c){
  return CanvasPoint(c.x * f, c.y*f,f * c.texturePoint);
}
