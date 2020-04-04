#include "CanvasPoint.h"


CanvasPoint::CanvasPoint()
{
    texturePoint = TexturePoint(-1,-1);
}

CanvasPoint::CanvasPoint(float xPos, float yPos)
{
  x = xPos;
  y = yPos;
  depth = 0.0;
  brightness = 1.0;
  texturePoint = TexturePoint(-1,-1);
}

CanvasPoint::CanvasPoint(float xPos, float yPos, float pointDepth)
{
  x = xPos;
  y = yPos;
  depth = pointDepth;
  brightness = 1.0;
  texturePoint = TexturePoint(-1,-1);
}

CanvasPoint CanvasPoint::operator-(const CanvasPoint& c){
  return CanvasPoint(x-c.x, y-c.y, depth-c.depth);
}

CanvasPoint CanvasPoint::operator+(const CanvasPoint& c){
  return CanvasPoint(x+c.x, y+c.y, depth+c.depth);
}

CanvasPoint CanvasPoint::operator/(const float f){
  return CanvasPoint(x/f, y/f, depth/f);
}

CanvasPoint::CanvasPoint(float xPos, float yPos, float pointDepth, float pointBrightness)
{
  x = xPos;
  y = yPos;
  depth = pointDepth;
  brightness = pointBrightness;
  texturePoint = TexturePoint(-1,-1);
}

CanvasPoint::CanvasPoint(float xPos, float yPos, TexturePoint tp){
  x = xPos;
  y = yPos;
  texturePoint = tp;
}
/*
CanvasPoint CanvasPoint::operator-(const CanvasPoint& c){
  return CanvasPoint(x-c.x, y-c.y, texturePoint - c.texturePoint);
}

CanvasPoint CanvasPoint::operator+(const CanvasPoint& c){
  return CanvasPoint(x+c.x, y+c.y, texturePoint + c.texturePoint);
}

CanvasPoint CanvasPoint::operator/(const float f){
  return CanvasPoint(x/f,y/f,texturePoint/f);
}

*/
std::ostream& operator<<(std::ostream& os, const CanvasPoint& point)
{
    os << "(" << point.x << ", " << point.y << ", " << point.depth << ") " << point.brightness << std::endl;
    return os;
}
CanvasPoint operator*(float f, CanvasPoint c){
  return CanvasPoint(c.x * f, c.y*f, f*c.depth);
}
