#pragma once

#include <glm/glm.hpp>
#include <iostream>

class RayTriangleIntersection
{
  public:
    glm::vec3 intersectionPoint;
    float distance;
    ModelTriangle intersectedTriangle;

    RayTriangleIntersection()
    {
    }

    RayTriangleIntersection(glm::vec3 point, float d, ModelTriangle triangle)
    {
        intersectionPoint = point;
        distance = d;
        intersectedTriangle = triangle;
    }
};

// std::ostream& operator<<(std::ostream& os, const RayTriangleIntersection& intersection)
// {
//     os << "Intersection is at " << intersection.intersectionPoint << " on triangle " << intersection.intersectedTriangle << " at a distance of " << intersection.distanceFromCamera << std::endl;
//     return os;
// }
