#include "Colour.h"


Colour::Colour()
{
}

Colour::Colour(int r, int g, int b)
{
  name = "";
  red = r;
  green = g;
  blue = b;
}

Colour::Colour(std::string n, int r, int g, int b)
{
  name = n;
  red = r;
  green = g;
  blue = b;
}

Colour::Colour(uint32_t packed){
  blue  = (packed & 0x000000ff);
  green = (packed & 0x0000ff00) >> 8;
  red   = (packed & 0x00ff0000) >> 16;
}


uint32_t Colour::pack(){
  return (255<<24) + (red<<16) + (green<<8) + blue;
}
// Changed
uint32_t Colour::pack(glm::vec3 brightness){
  int bred   = std::min((int) (red   *brightness.x),255);
  int bgreen = std::min((int) (green *brightness.y),255);
  int bblue  = std::min((int) (blue  *brightness.z),255);
  return (255<<24) + (bred<<16) + (bgreen<<8) + bblue;
}

uint32_t Colour::pack(float brightness){
  int bred   = std::min((int) (red   *brightness),255);
  int bgreen = std::min((int) (green *brightness),255);
  int bblue  = std::min((int) (blue  *brightness),255);
  return (255<<24) + (bred<<16) + (bgreen<<8) + bblue;


}



Colour Colour::operator+(const Colour& b){
  return Colour(std::min(red+b.red,255),
                std::min(green+b.green,255),
                std::min(blue+b.blue,255));
}

std::ostream& operator<<(std::ostream& os, const Colour& colour)
{
    os << colour.name << " [" << colour.red << ", " << colour.green << ", " << colour.blue << "]" << std::endl;
    return os;
}

uint32_t addColour(uint32_t a, uint32_t b){
  return (Colour(a) + Colour(b)).pack();
}