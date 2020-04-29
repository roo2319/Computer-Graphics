#include "TexturePoint.h"


TexturePoint::TexturePoint()
{
}

TexturePoint::TexturePoint(float xPos, float yPos)
{
  x = xPos;
  y = yPos;
}


TexturePoint TexturePoint::operator-(const TexturePoint& t){
  return TexturePoint(this->x-t.x, this->y-t.y);
}

TexturePoint TexturePoint::operator+(const TexturePoint& t){
  return TexturePoint(this->x+t.x, this->y+t.y);
}
TexturePoint TexturePoint::operator/(const float f){
  return TexturePoint(this->x/f,this->y/f);
}

std::ostream& operator<<(std::ostream& os, const TexturePoint& point)
{
    os << "(" << point.x << ", " << point.y << ")" << std::endl;
    return os;
}
TexturePoint operator*(float f, TexturePoint t){
  return TexturePoint(t.x * f, t.y*f);
}
bool operator==(TexturePoint a, TexturePoint b){
  return (a.x == b.x)
      && (a.y == b.y);
}
