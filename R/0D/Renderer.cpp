#include <Utils.h>
#include <ModelTriangle.h>
#include <CanvasTriangle.h>
#include <RayTriangleIntersection.h>
#include <DrawingWindow.h>
#include <glm/glm.hpp>
#include <fstream>
#include <vector>
#include <string>
#include <unordered_map>

using namespace std;
using namespace glm;

#define WIDTH 320
#define HEIGHT 240

vector<vector<uint32_t>> readPPM(const char * filename);
unordered_map<string,Colour> readMTL(const char* filename);
vector<ModelTriangle> readOBJ(const char* filename,unordered_map<string,Colour> mtls, float scale);
void drawWireframe(vector<ModelTriangle> model);
void drawRasterised(vector<ModelTriangle> model);
void drawRaytraced(vector<ModelTriangle> model);
bool closestIntersection(vec3 start, vec3 dir, vector<ModelTriangle> triangles,RayTriangleIntersection* intersection);
bool inPlane(CanvasPoint points[3]);


void four_colour();
void drawppm();
void draw();
void redNoise();
void greyscale();
void update();
void handleEvent(SDL_Event event);


unordered_map<string,Colour> materials = readMTL("cornell-box.mtl");
vector<vector<uint32_t>> image = readPPM("texture.ppm");
vector<ModelTriangle> model = readOBJ("cornell-box.obj",materials,1);
DrawingWindow window = DrawingWindow(WIDTH, HEIGHT, false);
vec3 camera = vec3(0,0,-5);
mat3 rotation = mat3(1.0f);
float focal = HEIGHT/2;


void updateRotation(mat3* rotation, float X, float Y, float Z){
  mat3 xrot = mat3(1,0,0,0,cos(X),sin(X),0,-sin(X),cos(X));
  mat3 yrot = mat3(cos(Y),0,-sin(Y),0,1,0,sin(Y),0,cos(Y));
  mat3 zrot = mat3(cos(Z),sin(Z),0,-sin(Z),cos(Z),0,0,0,1);
  *rotation = *rotation * xrot * yrot * zrot;
}


int main(int argc, char* argv[])
{
  SDL_Event event;
  while(true)
  {
    
    // We MUST poll for events - otherwise the window will freeze !
    if(window.pollForInputEvents(&event)) handleEvent(event);
    update();
    window.clearPixels();
    draw();
    // Need to render the frame at the end, or nothing actually gets shown on the screen !
    window.renderFrame();
  }
}

void draw()
{
  drawRaytraced(model);
  cout << "frame" << endl;
}

vector<vector<uint32_t>> readPPM(const char * filename){
  ifstream f;
  string line;
  vector<vector<uint32_t>> image;
  f.open( filename, ios::binary);
  if (!f.is_open()){
    cout << "Failed to open ppm" << endl;
    return image;
  }
  while (getline(f,line)) if (line[0] == 'P' and line[1] == '6') break;
  cout << "Read P6" << endl;
  while (getline(f,line)) if (line[0] != '#') break;
  int width  = stoi(line.substr(0,line.find(' ')));
  int height = stoi(line.substr(line.find(' ')));
  cout << width << endl;
  cout << height << endl;
  while (getline(f,line)) if (line[0] != '#') break;
  int maxval = stoi(line);
  cout << maxval << endl;
  // int bytesPerPixel = maxval < 256 ? 1 : 2;
  for(int y = 0; y < height; y++){
    vector<uint32_t> row;
    for (int x = 0; x < width; x++){

      Colour c;
      c.red   = f.get();
      c.green = f.get(); 
      c.blue  = f.get();
      // for(int d = 0; d < bytesPerPixel; d++){
      //   val <<= 8;
      //   val += f.get();
      // }
      row.push_back(c.pack());
    }
    image.push_back(row);
  }
  f.close();
  return image;
}

unordered_map<string,Colour> readMTL(const char* filename){
  ifstream f;
  string line;
  string name;
  unordered_map<string,Colour> materials;
  f.open( filename, ios::in);
  while (getline(f,line)) {
    if (line.find("newmtl") != string::npos) {
      name = line.substr(line.find(' ')+1);
    }
    if (line.find("Kd") != string::npos){
      string* c = split(line,' ');
      int r = round(255 * stof(c[1]));
      int g = round(255 * stof(c[2]));
      int b = round(255 * stof(c[3]));
      materials[name] = Colour(r,g,b);
      // cout << materials[name] << endl;
      } 
    }
  return materials;
}

vector<ModelTriangle> readOBJ(const char* filename,unordered_map<string,Colour> mtls, float scale){
  vector<ModelTriangle> triangles;
  vector<vec3> points;
  ifstream f;
  string line;
  Colour current_colour = Colour(255,255,255);
  f.open(filename, ios::in);
  while (getline(f,line)) {
    if (line.find("usemtl") != string::npos){
      string material = split(line, ' ')[1];
      if (!(mtls.find(material) == mtls.end())) current_colour =  mtls[material];
    }
    else if (line[0] == 'v') {
      string* toks = split(line,' ');
      // Z is made negative so image is on positive side of Z, flip y aswell 
      points.push_back(vec3(stof(toks[1])*scale,stof(toks[2])*-scale,stof(toks[3])*-scale));
    }
    else if (line[0] == 'f') {
      string* toks = split(line,' ');
      vec3 first = points.at(abs(stoi(split(toks[1],'/')[0])-1));
      vec3 second = points.at(abs(stoi(split(toks[2],'/')[0])-1)); 
      vec3 third = points.at(abs(stoi(split(toks[3],'/')[0])-1));
      ModelTriangle triangle = ModelTriangle(first,second,third,current_colour);
      triangles.push_back(triangle);
    }
  }

  return triangles; 
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
//Fix the focal PLANE! (NOT POINT)
CanvasPoint project(vec3 point, float focal, vec3 camera, mat3 rotation){
  //
  vec3 d = point-camera;
  int x = round(-focal * (d.x/d.z));
  int y = round(focal * (d.y/d.z));
  return CanvasPoint(x+WIDTH/2,y+HEIGHT/2);
}

void drawWireframe(vector<ModelTriangle> model){
  //Image plane = 0,0,0
  CanvasPoint first,second,third;
  Colour white = Colour(255,255,255);
  for(unsigned int i = 0; i<model.size();i++){
    first = project(model[i].vertices[0],focal,camera,rotation);
    second = project(model[i].vertices[1],focal,camera,rotation);
    third = project(model[i].vertices[2],focal,camera,rotation);
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
    first = project(model[i].vertices[0],focal,camera,rotation);
    second = project(model[i].vertices[1],focal,camera,rotation);
    third = project(model[i].vertices[2],focal,camera,rotation);
    CanvasPoint points[3] = {first, second, third};
    if (inPlane(points)){
      filled(window,first,second,third,model[i].colour);
    }
  }
}

void drawRaytraced(vector<ModelTriangle> model){
  RayTriangleIntersection intersection;
  vec3 dir;
  for(int y=0; y<window.height ;y++) {
    for(int x=0; x<window.width ;x++) {
      dir = vec3(x-WIDTH/2,y-HEIGHT/2,focal);
      if (closestIntersection(camera,dir*rotation,model,&intersection)){
        window.setPixelColour(x,y,intersection.intersectedTriangle.colour.pack());
      }
    }
  }

}

bool closestIntersection(vec3 start, vec3 dir,
                         vector<ModelTriangle> triangles,
                         RayTriangleIntersection* intersection){
  float bestT = 1000;
  for (unsigned int i = 0; i < triangles.size(); i++){
    ModelTriangle triangle = triangles[i];
    vec3 e1 = triangle.vertices[1] - triangle.vertices[0];
    vec3 e2 = triangle.vertices[2] - triangle.vertices[0];
    vec3  b = start - triangle.vertices[0];
    mat3 A(-dir,e1,e2);
    vec3 x = glm::inverse(A) * b; // distance , u , v
    if(x.x > 0 && x.x < bestT && x.y > 0 && x.z > 0 && x.y+x.z < 1){
      bestT = x.x;
      *intersection = RayTriangleIntersection(start,x.x,triangle);
    }
  } 
    return bestT < 1000;
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

void handleEvent(SDL_Event event)
{
  if(event.type == SDL_KEYDOWN) {
    if(event.key.keysym.sym == SDLK_LEFT){
      cout << "LEFT" << endl;
      updateRotation(&rotation,0,0.2,0);
    }  
    else if(event.key.keysym.sym == SDLK_RIGHT) {
      cout << "Right" << endl;
      updateRotation(&rotation,0,-0.2,0);
    }  
    else if(event.key.keysym.sym == SDLK_UP) {
      cout << "UP" << endl;
      updateRotation(&rotation,-0.2,0,0);
    }  
    else if(event.key.keysym.sym == SDLK_DOWN) {
      cout << "DOWN" << endl;
      updateRotation(&rotation,0.2,0,0);
    }  
    else if(event.key.keysym.sym == SDLK_j) {
      stroked(window, CanvasPoint(rand()%WIDTH,rand()%HEIGHT),
              CanvasPoint(rand()%WIDTH,rand()%HEIGHT),
              CanvasPoint(rand()%WIDTH,rand()%HEIGHT),
              Colour(rand()%255,rand()%255,rand()%255));
      cout << "J" << endl;
    }
    else if(event.key.keysym.sym == SDLK_f) {
      filled(window,CanvasPoint(rand()%WIDTH,rand()%HEIGHT),
              CanvasPoint(rand()%WIDTH,rand()%HEIGHT),
              CanvasPoint(rand()%WIDTH,rand()%HEIGHT),
              Colour(rand()%255,rand()%255,rand()%255));
      cout << "f" << endl;
    }
    else if (event.key.keysym.sym == SDLK_t){
      texturedTriangle(window,image,CanvasPoint(rand()%WIDTH,rand()%HEIGHT),
              CanvasPoint(rand()%WIDTH,rand()%HEIGHT),
              CanvasPoint(rand()%WIDTH,rand()%HEIGHT));
      cout << "t" << endl;

    }
    else if(event.key.keysym.sym == SDLK_c){
        window.clearPixels();
    }
    else if(event.key.keysym.sym == SDLK_w){
      cout << "moving camera up" << endl;
      camera -= vec3(0,1,0) * rotation;
    }
    else if(event.key.keysym.sym == SDLK_s){
      cout << "moving camera down" << endl;
      camera += vec3(0,1,0) * rotation;
    }
    else if(event.key.keysym.sym == SDLK_a){
      cout << "moving camera left" << endl;
      camera -= vec3(1,0,0) * rotation;
    }
    else if(event.key.keysym.sym == SDLK_d){
      cout << "moving camera right" << endl;
      camera += vec3(1,0,0) * rotation;
    }
    else if(event.key.keysym.sym == SDLK_q){
      cout << "moving camera forward" << endl;
      camera += vec3(0,0,1)*rotation  ;
    }
    else if(event.key.keysym.sym == SDLK_e){
      cout << "moving camera backward" << endl;
      camera -= vec3(0,0,1)*rotation;
    }
  }
  else if(event.type == SDL_MOUSEBUTTONDOWN) cout << "MOUSE CLICKED" << endl;
  else if(event.type == SDL_QUIT){
	exit(1);
  }
}
