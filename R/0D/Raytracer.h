#pragma once

#include <glm/glm.hpp>
#include <vector>
#include "ModelTriangle.h"
#include "RayTriangleIntersection.h"
#include "Camera.h"
#include "libs/sdw/DrawingWindow.h"



void drawRaytraced(std::vector<ModelTriangle> model, DrawingWindow window, Camera camera);