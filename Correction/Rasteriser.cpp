#include "Rasteriser.h"

CanvasPoint project(glm::vec3 point, Camera camera, int width, int height){
//   THIS SHOULD SUPPORT ROTATION
  glm::vec3 d = camera.rotation * (point-camera.position);
  int x = round(camera.focal * (d.x/d.z));
  int y = round(camera.focal * (d.y/d.z));
  CanvasPoint projected = CanvasPoint(x+width/2,height/2-y);
  //passing depth of vertices
  projected.depth = d.z;
  return projected;
}

//just checks if point in inside image plane
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
      stroked(window, first, second, third, model[i].colour);
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


void drawTextured(std::vector<ModelTriangle> model,DrawingWindow window, Camera camera, vector<vector<uint32_t>> image){
  //Image plane = 0,0,0
  CanvasPoint first,second,third;
  int width  = window.width;
  int height = window.height;
  for(unsigned int i = 0; i<model.size();i++){
    first = project(model[i].vertices[0],camera,width,height);
    second = project(model[i].vertices[1],camera,width,height);
    third = project(model[i].vertices[2],camera,width,height);
    CanvasPoint points[3] = {first, second, third};

    first.texturePoint  = model[i].texture[0];
    second.texturePoint = model[i].texture[1];
    third.texturePoint  = model[i].texture[2];

    if (inPlane(points,width,height)){
      texturedTriangle(window,image,first,second,third,i);
    }
  }
}
