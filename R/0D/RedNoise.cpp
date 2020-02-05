#include <ModelTriangle.h>
#include <CanvasTriangle.h>
#include <DrawingWindow.h>
#include <Utils.h>
#include <glm/glm.hpp>
#include <fstream>
#include <vector>

using namespace std;
using namespace glm;

#define WIDTH 320
#define HEIGHT 240

vector<float> interpolate(float from, float to, int numberOfValues);
vector<vec3> interpolate3(vec3 from, vec3 to, int numberOfValues);
vector<vector<vec3>> interpolate2d(vec3 top_left, vec3 top_right, vec3 bottom_left, vec3 bottom_right, int width, int height);
void four_colour();
void stroked(CanvasPoint first, CanvasPoint second, CanvasPoint third, Colour c);
void filled(CanvasPoint first, CanvasPoint second, CanvasPoint third, Colour c);
void line(CanvasPoint to, CanvasPoint from, Colour c);
void draw();
void redNoise();
void greyscale();
void update();
void handleEvent(SDL_Event event);


DrawingWindow window = DrawingWindow(WIDTH, HEIGHT, false);


int main(int argc, char* argv[])
{
  SDL_Event event;
  while(true)
  {
    
    // We MUST poll for events - otherwise the window will freeze !
    if(window.pollForInputEvents(&event)) handleEvent(event);
    update();
    draw();
    // Need to render the frame at the end, or nothing actually gets shown on the screen !
    window.renderFrame();
  }
}

void draw()
{
}

void drawPPM(){

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

void greyscale()
{
  vector<float> colour = interpolate(0,255,window.width);
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

void line(CanvasPoint to, CanvasPoint from, Colour c){
  float xDiff = to.x - from.x;
  float yDiff = to.y - from.y;
  int numberOfSteps =  ceil(std::max(abs(xDiff), abs(yDiff)));
  vector<float> X = interpolate(from.x,to.x, numberOfSteps);
  vector<float> Y = interpolate(from.y,to.y, numberOfSteps);
  for (int i = 0; i < numberOfSteps; i++)  window.setPixelColour(X.at(i), Y.at(i), c.pack());

}

void stroked(CanvasPoint first, CanvasPoint second, CanvasPoint third, Colour c){
  line(first,second,c);
  line(second,third,c);
  line(first,third,c);
}

void filled(CanvasPoint first, CanvasPoint second, CanvasPoint third, Colour c){
  if (first.y < second.y) swap(first,second);
  if (first.y < third.y ) swap(first,third );
  if (second.y < third.y) swap(second,third); 
  //First = top, Second = Middle, Third = Bottom;
  //Next find the intersection of first->third and y=second
  float scale = (first.y-second.y)/(first.y-third.y);
  CanvasPoint extra = CanvasPoint(round(first.x - scale*(first.x-third.x)),second.y);
  vector<float> firstToExtra = interpolate(first.x,extra.x,ceil(first.y-second.y));
  vector<float> firstToSecond = interpolate(first.x,second.x,ceil(first.y-second.y));
  vector<float> thirdToExtra = interpolate(third.x,extra.x,ceil(second.y-third.y));
  vector<float> thirdToSecond = interpolate(third.x,second.x,ceil(second.y-third.y));
  for (int i = 0; i <= first.y - second.y; i++){
    line(CanvasPoint(firstToExtra[i],first.y-i),CanvasPoint(firstToSecond[i],first.y-i),c);
  }
  for (int i = 0; i <= second.y - third.y; i++){
    line(CanvasPoint(thirdToExtra[i],third.y+i),CanvasPoint(thirdToSecond[i],third.y+i),c);
  }
  stroked(extra,second,third,c);
  stroked(first,second,extra,c);


}

vector<float> interpolate(float from, float to, int numberOfValues)
{
  vector<float> interpolated;
  for (int i = 0; i <= numberOfValues; i++){
    interpolated.push_back(from + i*((to-from)/numberOfValues));
  }
  return interpolated;
}

vector<vec3> interpolate3(vec3 from, vec3 to, int numberOfValues)
{
  vector<vec3> interpolated;
  vec3 step = ((float)1/numberOfValues) * (to - from);
  for (int  i = 0; i <= numberOfValues; i++){
    interpolated.push_back(from + (float(i) * step));
  }
  return interpolated;

}

vector<vector<vec3>> interpolate2d(vec3 top_left, vec3 top_right, vec3 bottom_left, vec3 bottom_right, int width, int height)
{
  vector<vec3> left = interpolate3(top_left,bottom_left,height);
  vector<vec3> right = interpolate3(top_right, bottom_right, height);
  vector<vector<vec3>> interpolated;
  for (int x = 0; x <= height; x++){
    interpolated.push_back(interpolate3(left.at(x),right.at(x),width));
  }
  return interpolated;

}



void update()
{
  // Function for performing animation (shifting artifacts or moving the camera)
}

void handleEvent(SDL_Event event)
{
  if(event.type == SDL_KEYDOWN) {
    if(event.key.keysym.sym == SDLK_LEFT) cout << "LEFT" << endl;
    else if(event.key.keysym.sym == SDLK_RIGHT) cout << "RIGHT" << endl;
    else if(event.key.keysym.sym == SDLK_UP) cout << "UP" << endl;
    else if(event.key.keysym.sym == SDLK_DOWN) cout << "DOWN" << endl;
    else if(event.key.keysym.sym == SDLK_j) {
      stroked(CanvasPoint(rand()%WIDTH,rand()%HEIGHT),
              CanvasPoint(rand()%WIDTH,rand()%HEIGHT),
              CanvasPoint(rand()%WIDTH,rand()%HEIGHT),
              Colour(rand()%255,rand()%255,rand()%255));
      cout << "J" << endl;
    }
    else if(event.key.keysym.sym == SDLK_f) {
      filled(CanvasPoint(rand()%WIDTH,rand()%HEIGHT),
              CanvasPoint(rand()%WIDTH,rand()%HEIGHT),
              CanvasPoint(rand()%WIDTH,rand()%HEIGHT),
              Colour(rand()%255,rand()%255,rand()%255));
      cout << "f" << endl;
    }
    else if(event.key.keysym.sym == SDLK_c){
        window.clearPixels();
    }
  }
  else if(event.type == SDL_MOUSEBUTTONDOWN) cout << "MOUSE CLICKED" << endl;
  else if(event.type == SDL_QUIT){
	exit(1);
  }
}
