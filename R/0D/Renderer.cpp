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
Model scene = Model(readOBJ("scene.obj",materials,1,1));
DrawingWindow window = DrawingWindow(WIDTH, HEIGHT, false);

/////////////////////////////////////////////////////////////////////////////////////////////////////////
vector<vector<uint32_t>> back = readPPM("ppm/bluebelt.ppm");
vector<vector<uint32_t>> tiger = readPPM("logo/texture.ppm");
vector<vector<glm::vec3>> swirl = readBump("swirl.ppm");
unordered_map<string,Colour> logomaterials = readMTL("logo/materials.mtl");
Model logo_ = Model(readOBJ("logo/logo.obj",logomaterials,0.02,299),vec3(-5.6,0,0));
Model logo = logo_;
/////////////////////////////////////////////////////////////////////////////////////////////////////////
vector<vector<uint32_t>> rockTexture = readPPM("blueStones/blue1.ppm");
unordered_map<string,Colour> rockmaterials = readMTL("blueStones/blueStone16.mtl");
Model rock_ = Model(readOBJ("blueStones/blueStone16.obj",rockmaterials,1,299),vec3(0,0,0));
Model rock = rock_;
glm::mat3 Rotation = mat3(1.0f);
glm::vec3 rockstart = vec3(rand()%20,rand()%20,30);
glm::vec3 rockdir = vec3( (rand()%20-10)/100, (rand()%20-10)/(100), (10+rand()%5)/10);

vector<Model> world = {scene,logo,rock};
/////////////////////////////////////////////////////////////////////////////////////////////////////////
void animation();
void start();
void logoRotate();
void orbit();

Camera camera = Camera(vec3(0,2,-6),mat3(1.0f),HEIGHT/2);
// Camera camera = Camera(vec3(0,106,-16),mat3(1.0f),HEIGHT/2);
//this camera pos overlooks the room

// Distance from centre of orbit
float orbitDist =  length(camera.position - vec3(0,2,1));
int renderer = 0;
int fna = 1;
int bounces = 0;
int sequence = 0;
int sloop = 0;
int SSMethod;
bool animate = false;

int main(int argc, char* argv[])
{
  for(unsigned int i = 0; i<world[0].faces.size(); i++){
    if (world[0].faces[i].nameBump == "swirl.ppm"){
      std::cout << "Set BUMP" << std::endl;
      world[0].faces[i].bump = &swirl;
    }
  }
  // start();
  logo_.texture(tiger);
  rock_.texture(rockTexture);
  SDL_Event event;
  while(true){
    // We MUST poll for events - otherwise the window will freeze !
    if(window.pollForInputEvents(&event)) handleEvent(event,window,camera,image,renderer,SSMethod,fna,bounces,animate);
    update();
    window.clearPixels();
    window.clearDepth();
    drawBack(window,WIDTH,HEIGHT,back);
    // logoRotate();
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
      drawWireframe(world,window,camera);
      break;

    case 1:

      drawRasterised(world,window,camera,tiger);
      break;

    case 2:

      drawRaytraced(world,window,camera,SSMethod,bounces);
      // drawRasterised(logo,window,camera,tiger);
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

void animation(){
  logo_.rotate(0,0.02,0);
  logo = logo_;
  logo.shift(vec3(0,100,10));
  world[1] = logo;

  if(rockstart[2]-camera.position[2] < 5){
    rockstart = vec3( rand()%20-10,   rand()%20-10,  (camera.position[2]+50+rand()%20) );
    rockdir = vec3( (rand()%1-2)/10, (rand()%1-2)/(10), (1+rand()%1));
  }
  rockstart -= rockdir ;
  rock_.rotate(rand()%2,rand()%2,rand()%2);
  rock = rock_;
  rock.shift(rockstart);
  world[2] = rock;

}
void update()
{
  animation();

  if (animate){
    switch(sequence){
      // Walk to portal, while taking turns
      case 0:
        camera.position = glm::vec3(0,1,0);
        sequence++;

      case 1:
        if (camera.position.z < 12){
          camera.forward(0.2);
        }
        else sequence++;
        break;

      case 2:
        if (sloop < 25){
          camera.updateRotation(0,0.0602,0);
          sloop++;
        }
        else {sequence++;sloop=0;}
        break;

      case 3:
        if (camera.position.x > -10){
          camera.forward(0.2);
        }
        else sequence++;
        break;

      case 4:
        if (sloop < 25){
          camera.updateRotation(0,-0.0601,0);
          sloop++;
        }
        else {sequence++;sloop=0;}
        break;

      case 5:
        if (camera.position.z < 23.5){
          camera.forward(0.2);
        }
        else sequence++;
        break;

      case 6:
        camera.position = glm::vec3(0,101,0.1);
        sequence++;
        break;

      case 7:
        if (camera.position.z < 10){
          camera.forward(0.2);
        }
        else{
          sequence++;
        }
        break;

      case 8:
        if (sloop < 25){
          camera.updateRotation(0,-0.1204,0);
          sloop++;
        }
        else {sequence++;sloop=0;}
        break;
      case 9:
        std::exit(0);


    }
    char filename[50];
    sprintf(filename,"captures/%d.ppm",fna);
    writePPM(filename, window);
    fna++;
  }
  // Function for performing animation (shifting artifacts or moving the camera)
    // camera.right(0.1);
}
