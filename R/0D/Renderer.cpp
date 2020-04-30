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
// Centered on origin
Model logo = Model(readOBJ("logo/logo.obj",logomaterials,0.02,299),vec3(-5.6,0,0));
/////////////////////////////////////////////////////////////////////////////////////////////////////////
vector<vector<uint32_t>> rockTexture = readPPM("blueStones/blue1.ppm");
unordered_map<string,Colour> rockmaterials = readMTL("blueStones/blueStone16.mtl");
Model rock = Model(readOBJ("blueStones/blueStone16.obj",rockmaterials,2,500),vec3(0,0,0));

vector<vector<uint32_t>> rockTexture2 = readPPM("blueStones/blue2.ppm");
unordered_map<string,Colour> rockmaterials2 = readMTL("blueStones/blueStone12.mtl");
Model rock2 = Model(readOBJ("blueStones/blueStone12.obj",rockmaterials2,2,500),vec3(0,0,0));

vector<vector<uint32_t>> rockTexture3 = readPPM("blueStones/blue3.ppm");
unordered_map<string,Colour> rockmaterials3 = readMTL("blueStones/blueStone3.mtl");
Model rock3 = Model(readOBJ("blueStones/blueStone3.obj",rockmaterials3,2,500),vec3(0,0,0));

vector<vector<uint32_t>> rockTexture4 = readPPM("blueStones/blue4.ppm");
unordered_map<string,Colour> rockmaterials4 = readMTL("blueStones/blueStone1.mtl");
Model rock4 = Model(readOBJ("blueStones/blueStone1.obj",rockmaterials4,2,500),vec3(0,0,0));

vector<Model> world = {scene,logo,rock,rock2,rock3,rock4};
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
  logo.transform(glm::vec3(-10,0,8),0,0,0);
  for(unsigned int i = 0; i<scene.faces.size(); i++){
    if (scene.faces[i].nameBump == "swirl.ppm"){
      std::cout << "Set BUMP" << std::endl;
      scene.faces[i].bump = &swirl;
    }
  }

  for(unsigned int i = 0; i<logo.faces.size(); i++){
    if( logo.faces[i].isTexture){
      logo.faces[i].image = &tiger;
    }
  }

  for(unsigned int i = 0; i<rock.faces.size(); i++){
    if( rock.faces[i].isTexture){
      rock.faces[i].image = &rockTexture;
    }
  }
  for(unsigned int i = 0; i<rock2.faces.size(); i++){
    if( rock2.faces[i].isTexture){
      rock2.faces[i].image = &rockTexture2;
    }
  }
  for(unsigned int i = 0; i<rock3.faces.size(); i++){
    if( rock3.faces[i].isTexture){
      rock3.faces[i].image = &rockTexture3;
    }
  }
  for(unsigned int i = 0; i<rock4.faces.size(); i++){
    if( rock4.faces[i].isTexture){
      rock4.faces[i].image = &rockTexture4;
    }
  }

  // start();
  // logo_.texture(tiger);
  // rock.texture(rockTexture);
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

      drawRasterised(world,window,camera);
      break;

    case 2:

      drawRaytraced(world,window,camera,SSMethod,bounces);
      // drawRasterised(world,window,camera,rockTexture);
      break;

  }
}
/*
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
// }*/

void animation(){
  world[0] = scene;
  logo.transform(vec3(0,0,0),0,0.02,0);
  world[1] = logo;

  // Model temp;
  rock.rockUpdate(camera.position[2]);
  rock.transform(vec3(0,0,0),(rand()%10)/100.0,(rand()%10)/100.0,(rand()%10)/100.0);
  Model temp = rock;
  temp.transform(rock.rockstart,0,0,0);
  world[2] = temp;;

  rock2.rockUpdate(camera.position[2]);
  rock2.transform(vec3(0,0,0),(rand()%10)/100.0,(rand()%10)/100.0,(rand()%10)/100.0);
  Model temp2 = rock2;
  temp.transform(rock2.rockstart,0,0,0);
  world[3] = temp;;

  rock3.rockUpdate(camera.position[2]);
  rock3.transform(vec3(0,0,0),(rand()%10)/100.0,(rand()%10)/100.0,(rand()%10)/100.0);
  Model temp3 = rock3;
  temp.transform(rock3.rockstart,0,0,0);
  world[4] = temp;;

  rock4.rockUpdate(camera.position[2]);
  rock4.transform(vec3(0,0,0),(rand()%10)/100.0,(rand()%10)/100.0,(rand()%10)/100.0);
  Model temp4 = rock4;
  temp.transform(rock4.rockstart,0,0,0);
  world[5] = temp;;
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
