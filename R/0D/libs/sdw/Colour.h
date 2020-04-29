#pragma once

#include <iostream>
#include <glm/glm.hpp>
#include <algorithm>

class Colour
{
  public:
    std::string name;
    int red;
    int green;
    int blue;

    float specularIndex;


    Colour();

    Colour(int r, int g, int b);
    Colour(int r, int g, int b, float s);
    Colour(std::string n, int r, int g, int b);
    Colour(std::string n, int r, int g, int b, float s);

    Colour(uint32_t packed);
    Colour operator+(const Colour& b);

    uint32_t pack();
    uint32_t pack(glm::vec3 brightness);
    uint32_t pack(float brightness);

};

std::ostream& operator<<(std::ostream& os, const Colour& colour);
Colour operator/(Colour colour,int a);
Colour operator*(glm::vec3 brightness,Colour colour);
uint32_t addColour(uint32_t a, uint32_t b);