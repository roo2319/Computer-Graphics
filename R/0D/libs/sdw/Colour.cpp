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

uint32_t Colour::pack(){
  return (255<<24) + (this->red<<16) + (this->green<<8) + this->blue;
}


std::ostream& operator<<(std::ostream& os, const Colour& colour)
{
    os << colour.name << " [" << colour.red << ", " << colour.green << ", " << colour.blue << "]" << std::endl;
    return os;
}
