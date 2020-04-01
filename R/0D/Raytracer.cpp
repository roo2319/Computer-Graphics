#include "Raytracer.h"

bool closestIntersection(glm::vec3 start, glm::vec3 dir,
                         std::vector<ModelTriangle> triangles,
                         RayTriangleIntersection* intersection){
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
      *intersection = RayTriangleIntersection(start,x.x,triangle);
    }
  } 
    return bestT < 1000;
}

void drawRaytraced(std::vector<ModelTriangle> model, DrawingWindow window, Camera camera){
  RayTriangleIntersection intersection;
  glm::vec3 dir;
  #pragma omp parallel for private(intersection, dir)
  for(int y=0; y<window.height ;y++) {
    for(int x=0; x<window.width ;x++) {
      dir = glm::vec3(x-window.width/2,y-window.height/2,camera.focal);
      if (closestIntersection(camera.position,dir*camera.rotation,model,&intersection)){
        window.setPixelColour(x,y,intersection.intersectedTriangle.colour.pack());
      }
    }
  }
}