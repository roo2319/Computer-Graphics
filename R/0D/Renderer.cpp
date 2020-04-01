#include "Utils.h"
#include "ModelTriangle.h"
#include "CanvasTriangle.h"
#include "EventHandler.h"
#include "RayTriangleIntersection.h"
#include "DrawingWindow.h"
#include "Camera.h"
#include "Raytracer.h"
#include <glm/glm.hpp>
#include <fstream>
#include <vector>
#include <string>
#include <unordered_map>

using namespace std;
using namespace glm;

#define WIDTH 320
#define HEIGHT 240

void drawWireframe(vector<ModelTriangle> model);
void drawRasterised(vector<ModelTriangle> model);
bool inPlane(CanvasPoint points[3]);


void four_colour();
void drawppm();
void draw();
void redNoise();
void greyscale();
void update();


unordered_map<string,Colour> materials = readMTL("cornell-box.mtl");
vector<vector<uint32_t>> image = readPPM("texture.ppm");
vector<ModelTriangle> model = readOBJ("cornell-box.obj",materials,1);
DrawingWindow window = DrawingWindow(WIDTH, HEIGHT, false);

Camera camera = Camera(vec3(0,0,-5),mat3(1.0f),HEIGHT/2);


int main(int argc, char* argv[])
{
  SDL_Event event;
  while(true)
  {
    
    // We MUST poll for events - otherwise the window will freeze !
    if(window.pollForInputEvents(&event)) handleEvent(event,window,camera,image);
    update();
    window.clearPixels();
    draw();
    // Need to render the frame at the end, or nothing actually gets shown on the screen !
    window.renderFrame();
  }
}

void draw()
{
  drawRaytraced(model,window,camera);
  cout << "frame" << endl;
}



void redNoise(){
  for(int y=0; y<window.height ;y++) {
    for(int x=0; x<window.width ;x++) {
      float red   = rand() % 255;
      float green = 0.0;
      float blue  = 0.0;
      uint32_t colour = (255<<24) + (int(red)<<16) + (int(green)<<8) + int(blue);
      window.setPixelColour(x, y, colour);
    }
  }
}

void drawppm(){
  vector<vector<uint32_t>> image = readPPM("texture.ppm");
  for(unsigned int y = 0; y < image.size(); y++){
    for (unsigned int x = 0; x < image.at(0).size(); x++){
      window.setPixelColour(x,y,image.at(y).at(x));
    }
  }
}

CanvasPoint project(vec3 point, Camera camera){
  
  vec3 d = point-camera.position;
  int x = round(-camera.focal * (d.x/d.z));
  int y = round(camera.focal * (d.y/d.z));
  return CanvasPoint(x+WIDTH/2,y+HEIGHT/2);
}

void drawWireframe(vector<ModelTriangle> model){
  //Image plane = 0,0,0
  CanvasPoint first,second,third;
  Colour white = Colour(255,255,255);
  for(unsigned int i = 0; i<model.size();i++){
    first = project(model[i].vertices[0],camera);
    second = project(model[i].vertices[1],camera);
    third = project(model[i].vertices[2],camera);
    CanvasPoint points[3] = {first, second, third};
    if (inPlane(points)){
      stroked(window, first, second, third, white);
    }
  }
}

bool inPlane(CanvasPoint points[3]){
  for(int i = 0; i<3; i++){
    if (points[i].x < 0 || points[i].x > WIDTH) return false;
    if (points[i].y < 0 || points[i].y > HEIGHT) return false;
  }
  return true;
}

void drawRasterised(vector<ModelTriangle> model){
  //Image plane = 0,0,0
  CanvasPoint first,second,third;
  for(unsigned int i = 0; i<model.size();i++){
    first = project(model[i].vertices[0],camera);
    second = project(model[i].vertices[1],camera);
    third = project(model[i].vertices[2],camera);
    CanvasPoint points[3] = {first, second, third};
    if (inPlane(points)){
      filled(window,first,second,third,model[i].colour);
    }
  }
}


void greyscale()
{
  vector<int> colour = interpolate(0,255,window.width);
  for(int y=0; y<window.height;y++) {
    for(int x=0; x<window.width;x++) {
      float red   = colour.at(x);
      float green = colour.at(x);
      float blue  = colour.at(x);
      uint32_t colour = (255<<24) + (int(red)<<16) + (int(green)<<8) + int(blue);
      window.setPixelColour(x, y, colour);
    }
  }
} 

void four_colour()
{
  vector<vector<vec3>> colour = interpolate2d(vec3(255,0,0),vec3(0,0,255),vec3(255,255,0),vec3(0,255,0),window.width,window.height);
  for(int y=0; y<window.height;y++) {
    for(int x=0; x<window.width;x++) {
      float red   = colour.at(y).at(x).x;
      float green = colour.at(y).at(x).y;
      float blue  = colour.at(y).at(x).z;
      uint32_t colour = (255<<24) + (int(red)<<16) + (int(green)<<8) + int(blue);
      window.setPixelColour(x, y, colour);
    }
  }
}


void update()
{
  // Function for performing animation (shifting artifacts or moving the camera)
}

