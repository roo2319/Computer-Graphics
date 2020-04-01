#pragma once

#include <algorithm>
#include "CanvasPoint.h"
#include "Colour.h"
#include "ModelTriangle.h"
#include <glm/glm.hpp>
#include <vector>
#include <unordered_map>
#include <fstream>
#include "DrawingWindow.h"

using std::vector;
std::string* split(std::string line, char delim);

// Interpolation
vector<int> interpolate(float from, float to, int numberOfValues);
vector<CanvasPoint> interpolate(CanvasPoint from, CanvasPoint to, int numberOfValues);
vector<glm::vec3> interpolate(glm::vec3 from, glm::vec3 to, int numberOfValues);
vector<vector<glm::vec3>> interpolate2d(glm::vec3 top_left, glm::vec3 top_right, glm::vec3 bottom_left, glm::vec3 bottom_right, int width, int height);

// Drawing
void line(DrawingWindow window, CanvasPoint to, CanvasPoint from, Colour c);
void texturedLine(DrawingWindow window, CanvasPoint to, CanvasPoint from, vector<vector<uint32_t>> texture);
void stroked(DrawingWindow window, CanvasPoint first, CanvasPoint second, CanvasPoint third, Colour c);
void filled(DrawingWindow window, CanvasPoint first, CanvasPoint second, CanvasPoint third, Colour c);
void texturedTriangle(DrawingWindow window, vector<vector<uint32_t>> image, CanvasPoint first, CanvasPoint second, CanvasPoint third);

// Parsing
vector<vector<uint32_t>> readPPM(const char * filename);
std::unordered_map<std::string,Colour> readMTL(const char* filename);
vector<ModelTriangle> readOBJ(const char* filename,std::unordered_map<std::string,Colour> mtls, float scale);

// Test images
void redNoise(DrawingWindow window);
void drawppm(DrawingWindow window);
void greyscale(DrawingWindow window);
void four_colour(DrawingWindow window);