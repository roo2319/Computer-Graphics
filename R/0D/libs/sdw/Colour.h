#pragma once

#include <iostream>

class Colour
{
  public:
    std::string name;
    int red;
    int green;
    int blue;

    Colour();

    Colour(int r, int g, int b);

    Colour(std::string n, int r, int g, int b);

    Colour(uint32_t packed);

    uint32_t pack();

};

std::ostream& operator<<(std::ostream& os, const Colour& colour);