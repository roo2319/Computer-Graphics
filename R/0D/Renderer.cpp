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
  // drawRasterised(model,window,camera);
  cout << "frame" << endl;
}






void update()
{
  // Function for performing animation (shifting artifacts or moving the camera)
}

