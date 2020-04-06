#include "Raytracer.h"

glm::vec3 pointLight = glm::vec3(0,3.5,2);
glm::vec3 lightColour = 50.f * glm::vec3(1,1,1);
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
    return 0.5f * indirectLighting; //Shadow
  }
  
  glm::vec3 n = i.intersectedTriangle.normal;
  float percent = std::max(glm::dot(glm::normalize(r),n),0.f);
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

bool calculateIntersectionWithBounces(DrawingWindow window,
                         Camera camera, int x, int y,
                         std::vector<ModelTriangle> triangles,
                         RayTriangleIntersection& intersection){
  glm::vec3 dir = glm::vec3(x-window.width/2,window.height/2-y,camera.focal) * camera.rotation;
  glm::vec3 n;
  if (closestIntersection(camera.position,dir,triangles,intersection)){
    if (intersection.intersectedTriangle.colour.name == "Mirror"){
      n = intersection.intersectedTriangle.normal;
      dir = glm::normalize(dir);                     
      dir = dir - 2*(glm::dot(dir,n))*n;
      if (closestIntersection(intersection.intersectionPoint,dir,triangles,intersection,intersection.intersectedTriangle)){
        return true;
      }
    }
    else{
      return true;
    }
  }
  return false;
}

void simple(DrawingWindow window,std::vector<ModelTriangle> model,int x, int y, Camera camera){
  RayTriangleIntersection intersection;
  if (calculateIntersectionWithBounces(window,camera,x,y,model,intersection)){
    window.setPixelColour(x,y,intersection.intersectedTriangle.colour.pack(Lighting(intersection,model))); 
  }
}

void NbyNGrid(DrawingWindow window,std::vector<ModelTriangle> model,int x, int y, float n, Camera camera){
  RayTriangleIntersection intersection;
  float offset = 1 / (2*n);
  glm::vec3 dir;
  Colour colour = Colour(0,0,0);
  int count = 0;
  for (int i = 1; i<n; i++){
    for (int j = 1; j<n; j++){
      dir = glm::vec3(x-window.width/2+(i * 1/n)-offset,window.height/2-y-(j*1/n)-offset,camera.focal);
      if (calculateIntersectionWithBounces(window,camera,x,y,model,intersection)){
        colour = colour + Lighting(intersection,model) * intersection.intersectedTriangle.colour;
        count++;
      }
    }
  }
  if (count != 0) window.setPixelColour(x,y,(colour/count).pack());

}

void samplePixels(DrawingWindow window,std::vector<ModelTriangle> model,int x, int y, Camera camera, int SSMethod){
  switch (SSMethod){
    case 0: //Simple
      simple(window,model,x,y,camera);
      break;
    case 1: //2by2 grid
      NbyNGrid(window,model,x,y,2.f,camera);
      break;
    case 2: //4by4 grid
      NbyNGrid(window,model,x,y,4.f,camera);
      break;
  }
}

void drawRaytraced(std::vector<ModelTriangle> model, DrawingWindow window, Camera camera,int SSMethod){
  #pragma omp parallel for
  for(int y=0; y<window.height ;y++) {
    for(int x=0; x<window.width ;x++) {
      samplePixels(window,model,x,y,camera,SSMethod);
      }
    }
  }
