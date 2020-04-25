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

#define WIDTH 640
#define HEIGHT 480

// #define WIDTH 960
// #define HEIGHT 720

void draw();
void update();

unordered_map<string,Colour> materials = readMTL("scene.mtl");
vector<vector<uint32_t>> image = readPPM("texture.ppm");
vector<ModelTriangle> model = readOBJ("cornell-box.obj",materials,1);
DrawingWindow window = DrawingWindow(WIDTH, HEIGHT, false);

void orbit();

Camera camera = Camera(vec3(0,2,-6),mat3(1.0f),HEIGHT/2);
// Distance from centre of orbit
float orbitDist =  length(camera.position - vec3(0,2,1));
int renderer = 2;
int fna = 1;
int bounces = 0;
int sequence = 0;
int sloop = 0;
int SSMethod;
bool animate = false;

int main(int argc, char* argv[])
{
  SDL_Event event;
  while(true){
    // We MUST poll for events - otherwise the window will freeze !
    if(window.pollForInputEvents(&event)) handleEvent(event,window,camera,image,renderer,SSMethod,fna,bounces,animate);
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
      drawRaytraced(model,window,camera,SSMethod,bounces);
      break;

  }
}

// void orbit(){
//   drawRaytraced(model,window,camera,SSMethod,bounces);
//   camera.lookat(glm::vec3(0,2,1));
//   camera.right(0.1);

//   // Ensures we have an   char filename[50];
//   sprintf(filename,"captures/%d.ppm",fna);
//   writePPM(filename, window);orbit rather than spiral
//   float distfromcentre = length(camera.position - vec3(0,2,1));
//   float scale = distfromcentre/orbitDist;
//   camera.forward(scale - 1);

//   // Write output
//   char filename[50];
//   sprintf(filename,"captures/%d.ppm",fna);
//   writePPM(filename, window);
//   fna++;
// }

void update()
{
  if (animate){
    switch(sequence){
      // Walk to portal, while taking turns
      case 0: 
        camera.position = glm::vec3(0,1,0);
        sequence++;

      case 1: 
        if (camera.position.z < 12){
          camera.forward(0.1);
        }
        else sequence++;
        break;

      case 2:
        if (sloop < 50){
          camera.updateRotation(0,0.0301,0);
          sloop++;
        }
        else {sequence++;sloop=0;}
        break;

      case 3: 
        if (camera.position.x > -10){
          camera.forward(0.1);
        }
        else sequence++;
        break;

      case 4:
        if (sloop < 50){
          camera.updateRotation(0,-0.0301,0);
          sloop++;
        }
        else {sequence++;sloop=0;}
        break;
      
      case 5: 
        if (camera.position.z < 23.5){
          camera.forward(0.1);
        }
        else sequence++;
        break;

      case 6: 
        camera.position = glm::vec3(0,101,0.1);
        sequence++;
        break;      

      case 7: 
        if (camera.position.z < 10){
          camera.forward(0.3);
        }
        else{
          sequence++;
        }
        break;

      case 8:
        if (sloop < 50){
          camera.updateRotation(0,-0.0602,0);
          sloop++;
        }
        else {sequence++;sloop=0;}
        break;


    }
    char filename[50];
    sprintf(filename,"captures/%d.ppm",fna);
    writePPM(filename, window);
    fna++;
  }
  // Function for performing animation (shifting artifacts or moving the camera)
    // camera.right(0.1);
}
