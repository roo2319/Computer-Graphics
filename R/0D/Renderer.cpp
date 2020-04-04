#include "Utils.h"
#include "ModelTriangle.h"
#include "CanvasTriangle.h"
#include "EventHandler.h"
#include "RayTriangleIntersection.h"
#include "DrawingWindow.h"
#include "Camera.h"
#include "Raytracer.h"
#include "Rasteriser.h"
#include <glm/glm.hpp>
#include <fstream>
#include <vector>
#include <string>
#include <unordered_map>

using namespace std;
using namespace glm;

#define WIDTH 320
#define HEIGHT 240

// #define WIDTH 960
// #define HEIGHT 720

void draw();
void update();

unordered_map<string,Colour> materials = readMTL("cornell-box.mtl");
vector<vector<uint32_t>> image = readPPM("texture.ppm");
vector<ModelTriangle> model = readOBJ("cornell-box.obj",materials,1);
DrawingWindow window = DrawingWindow(WIDTH, HEIGHT, false);

void orbit();

Camera camera = Camera(vec3(0,2,-6),mat3(1.0f),HEIGHT/2);
// Distance from centre of orbit
float orbitDist =  length(camera.position - vec3(0,2,1));
int renderer = 0;
int fna = 1;

int main(int argc, char* argv[])
{
  SDL_Event event;
  while(true){
    // We MUST poll for events - otherwise the window will freeze !
    if(window.pollForInputEvents(&event)) handleEvent(event,window,camera,image,renderer,fna);
    update();
    window.clearPixels();
    window.clearDepth();
    draw();
    // Need to render the frame at the end, or nothing actually gets shown on the screen !
    window.renderFrame();
  }
}

void draw()
{
  // orbit();
  switch (renderer){
    case 0:
      drawWireframe(model,window,camera);
      break;

    case 1:
      drawRasterised(model,window,camera);
      break;

    case 2:
      drawRaytraced(model,window,camera);
      break;

  }
}

void orbit(){
  drawRaytraced(model,window,camera);
  camera.lookat(glm::vec3(0,2,1));
  camera.right(0.1);

  // Ensures we have an orbit rather than spiral
  float distfromcentre = length(camera.position - vec3(0,2,1));
  float scale = distfromcentre/orbitDist;
  camera.forward(scale - 1);

  // Write output
  char filename[50];
  sprintf(filename,"captures/%d.ppm",fna);
  writePPM(filename, window);
  fna++;
}

void update()
{
  // Function for performing animation (shifting artifacts or moving the camera)
}
