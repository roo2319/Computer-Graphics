#include "Raytracer.h"

glm::vec3 pointLight = glm::vec3(0,3.5,2);
glm::vec3 lightColour = 30.f * glm::vec3(1,1,1);
glm::vec3 indirectLighting = 0.25f * glm::vec3(1,1,1);
float pi = 3.1415f; //Approximation

glm::vec3 Lighting(const RayTriangleIntersection& i,std::vector<ModelTriangle> triangles);
bool closestIntersection(glm::vec3 start, glm::vec3 dir,
                         std::vector<ModelTriangle> triangles,
                         RayTriangleIntersection& intersection);
bool closestIntersection(glm::vec3 start, glm::vec3 dir,
                         std::vector<ModelTriangle> triangles,
                         RayTriangleIntersection& intersection,
                         ModelTriangle self);

glm::vec3 Lighting(const RayTriangleIntersection& i,std::vector<ModelTriangle> triangles){
  RayTriangleIntersection nearestSurface;
  glm::vec3 r = pointLight - i.intersectionPoint;
  bool found = closestIntersection(i.intersectionPoint,glm::normalize(r),triangles,nearestSurface,i.intersectedTriangle);
  if ( found && nearestSurface.distance < glm::length(r)){
    return 0.5f * indirectLighting;
  }
  
  glm::vec3 n = glm::cross(i.intersectedTriangle.vertices[0] - i.intersectedTriangle.vertices[1] ,
                           i.intersectedTriangle.vertices[2] - i.intersectedTriangle.vertices[0]);
  float percent = std::max(glm::dot(glm::normalize(r),glm::normalize(n)),0.f);
  return indirectLighting + (lightColour * (percent/(4*pi*glm::dot(r,r))));

}

bool closestIntersection(glm::vec3 start, glm::vec3 dir,
                         std::vector<ModelTriangle> triangles,
                         RayTriangleIntersection& intersection){
  float bestT = 1000;
  for (unsigned int i = 0; i < triangles.size(); i++){
    ModelTriangle triangle = triangles[i];
    glm::vec3 e1 = triangle.vertices[1] - triangle.vertices[0];
    glm::vec3 e2 = triangle.vertices[2] - triangle.vertices[0];
    glm::vec3  b = start - triangle.vertices[0];
    glm::mat3 A(-dir,e1,e2);
    glm::vec3 x = glm::inverse(A) * b; // distance , u , v
    if(x.x > 0 && x.x < bestT && x.y > 0 && x.z > 0 && x.y+x.z < 1){
      bestT = x.x;
      intersection = RayTriangleIntersection(triangle.vertices[0] + e1 * x.y + e2 * x.z ,x.x,triangle);
    }
  } 
    return bestT < 1000;
}

bool closestIntersection(glm::vec3 start, glm::vec3 dir,
                         std::vector<ModelTriangle> triangles,
                         RayTriangleIntersection& intersection,
                         ModelTriangle self){
  float bestT = 1000;
  for (unsigned int i = 0; i < triangles.size(); i++){
    ModelTriangle triangle = triangles[i];
    if (triangles[i] == self){
      continue;
    }
    glm::vec3 e1 = triangle.vertices[1] - triangle.vertices[0];
    glm::vec3 e2 = triangle.vertices[2] - triangle.vertices[0];
    glm::vec3  b = start - triangle.vertices[0];
    glm::mat3 A(-dir,e1,e2);
    glm::vec3 x = glm::inverse(A) * b; // distance , u , v
    if(x.x > 0 && x.x < bestT && x.y > 0 && x.z > 0 && x.y+x.z < 1){
      bestT = x.x;
      intersection = RayTriangleIntersection(triangle.vertices[0] + e1 * x.y + e2 * x.z ,x.x,triangle);
    }
  } 
    return bestT < 1000;
}
void drawRaytraced(std::vector<ModelTriangle> model, DrawingWindow window, Camera camera){
  RayTriangleIntersection intersection;
  glm::vec3 dir;
  glm::vec3 brightness;
  #pragma omp parallel for private(intersection, dir, brightness)
  for(int y=0; y<window.height ;y++) {
    for(int x=0; x<window.width ;x++) {
      dir = glm::vec3(x-window.width/2,window.height/2-y,camera.focal);
      if (closestIntersection(camera.position,dir*camera.rotation,model,intersection)){
        brightness = Lighting(intersection,model);
        window.setPixelColour(x,y,intersection.intersectedTriangle.colour.pack(brightness));
      }
    }
  }
}