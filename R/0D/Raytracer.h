#pragma once

#include <glm/glm.hpp>
#include <vector>
#include "ModelTriangle.h"
#include "RayTriangleIntersection.h"
#include "Camera.h"
#include "libs/sdw/DrawingWindow.h"




bool closestIntersection(glm::vec3 start, glm::vec3 dir,
                         std::vector<ModelTriangle> triangles,
                         RayTriangleIntersection* intersection);
void drawRaytraced(std::vector<ModelTriangle> model, DrawingWindow window, Camera camera);