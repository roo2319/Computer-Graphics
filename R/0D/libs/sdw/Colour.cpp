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

uint32_t Colour::pack(glm::vec3 brightness){
  int bred   = std::min((int) (red/2  *brightness.x),255);
  int bgreen = std::min((int) (green/2*brightness.y),255);
  int bblue  = std::min((int) (blue/2 *brightness.z),255);
  return (255<<24) + (bred<<16) + (bgreen<<8) + bblue;
}



std::ostream& operator<<(std::ostream& os, const Colour& colour)
{
    os << colour.name << " [" << colour.red << ", " << colour.green << ", " << colour.blue << "]" << std::endl;
    return os;
}
