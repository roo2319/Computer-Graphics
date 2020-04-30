#include "Colour.h"


Colour::Colour()
{
}

// THERE IS NOT CAP ON COLOUR VALUES, THIS IS INTENDED
Colour::Colour(int r, int g, int b)
{
  name = "";
  red = r;
  green = g;
  blue = b;
  specularIndex = 0;
  isBump = false;

}

Colour::Colour(int r, int g, int b, float s)
{
  name = "";
  red = r;
  green = g;
  blue = b;
  specularIndex = 0;
  isBump = false;

}

Colour::Colour(std::string n, int r, int g, int b)
{
  name = n;
  red = r;
  green = g;
  blue = b;
  specularIndex = 0;
  isBump = false;

}

Colour::Colour(std::string n, int r, int g, int b, float s)
{
  name = n;
  red = r;
  green = g;
  blue = b;
  specularIndex = s;
  isBump = false;

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
  return (brightness * *this).pack();
}

uint32_t Colour::pack(float brightness){
  int bred   = std::min((int) (red   *brightness),255);
  int bgreen = std::min((int) (green *brightness),255);
  int bblue  = std::min((int) (blue  *brightness),255);
  return (255<<24) + (bred<<16) + (bgreen<<8) + bblue;


}


//This function is used to add colours, although it may return too large values
Colour Colour::operator+(const Colour& b){
  return Colour(red+b.red,
                green+b.green,
                blue+b.blue);
}

std::ostream& operator<<(std::ostream& os, const Colour& colour)
{
    os << colour.name << " [" << colour.red << ", " << colour.green << ", " << colour.blue << "]" << std::endl;
    return os;
}

// Int division for colours
Colour operator/(Colour colour, int a){
  return Colour(colour.red/a,
                colour.green/a,
                colour.blue/a);
}

Colour operator*(glm::vec3 brightness,Colour colour){
  int bred   = std::min((int) (colour.red   *brightness.x),255);
  int bgreen = std::min((int) (colour.green *brightness.y),255);
  int bblue  = std::min((int) (colour.blue  *brightness.z),255);
  return Colour(bred,bgreen,bblue);
}

uint32_t addColour(uint32_t a, uint32_t b){
  return (Colour(a) + Colour(b)).pack();
}