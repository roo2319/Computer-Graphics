#include "Rasteriser.h"

CanvasPoint project(glm::vec3 point, Camera camera, int width, int height){
//   THIS SHOULD SUPPORT ROTATION
  glm::vec3 d = point-camera.position;
  int x = round(-camera.focal * (d.x/d.z));
  int y = round(camera.focal * (d.y/d.z));
  return CanvasPoint(x+width/2,y+height/2);
}

bool inPlane(CanvasPoint points[3],int width,int height){
  for(int i = 0; i<3; i++){
    if (points[i].x < 0 || points[i].x > width) return false;
    if (points[i].y < 0 || points[i].y > height) return false;
  }
  return true;
}

void drawWireframe(std::vector<ModelTriangle> model,DrawingWindow window, Camera camera){
  //Image plane = 0,0,0
  CanvasPoint first,second,third;
  Colour white = Colour(255,255,255);
  int width  = window.width;
  int height = window.height;
  for(unsigned int i = 0; i<model.size();i++){
    first = project(model[i].vertices[0],camera,width,height);
    second = project(model[i].vertices[1],camera,width,height);
    third = project(model[i].vertices[2],camera,width,height);
    CanvasPoint points[3] = {first, second, third};
    if (inPlane(points,width,height)){
      stroked(window, first, second, third, white);
    }
  }
}


void drawRasterised(std::vector<ModelTriangle> model,DrawingWindow window, Camera camera){
  //Image plane = 0,0,0
  CanvasPoint first,second,third;
  int width  = window.width;
  int height = window.height;
  for(unsigned int i = 0; i<model.size();i++){
    first = project(model[i].vertices[0],camera,width,height);
    second = project(model[i].vertices[1],camera,width,height);
    third = project(model[i].vertices[2],camera,width,height);
    CanvasPoint points[3] = {first, second, third};
    if (inPlane(points,width,height)){
      filled(window,first,second,third,model[i].colour);
    }
  }
}
