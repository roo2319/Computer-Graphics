#include "Utils.h"

std::string* split(std::string line, char delim)
{
    int numberOfTokens = count(line.begin(), line.end(), delim) + 1;
    std::string *tokens = new std::string[numberOfTokens];
    int currentPosition = 0;
    int nextIndex = 0;
    for(int i=0; i<numberOfTokens ;i++) {
        nextIndex = line.find(delim, currentPosition);
        tokens[i] = line.substr(currentPosition,nextIndex-currentPosition);
        currentPosition = nextIndex + 1;
    }
    return tokens;
}
std::vector<int> interpolate(float from, float to, int numberOfValues)
{
std::vector<int> interpolated;
for (int i = 0; i <= numberOfValues; i++){
    interpolated.push_back(round(from + i*((to-from)/numberOfValues)));
}
return interpolated;
}

std::vector<CanvasPoint> interpolate(CanvasPoint from, CanvasPoint to, int numberOfValues){
std::vector<CanvasPoint> interpolated;
for (int i = 0; i <= numberOfValues; i++){
    interpolated.push_back(from + (i * (to - from)/numberOfValues));
}
return interpolated;
}

std::vector<glm::vec3> interpolate(glm::vec3 from, glm::vec3 to, int numberOfValues)
{
std::vector<glm::vec3> interpolated;
glm::vec3 step = ((float)1/numberOfValues) * (to - from);
for (int  i = 0; i <= numberOfValues; i++){
    interpolated.push_back(from + (float(i) * step));
}
return interpolated;

}

std::vector<std::vector<glm::vec3>> interpolate2d(glm::vec3 top_left, glm::vec3 top_right, glm::vec3 bottom_left, glm::vec3 bottom_right, int width, int height)
{
std::vector<glm::vec3> left = interpolate(top_left,bottom_left,height);
std::vector<glm::vec3> right = interpolate(top_right, bottom_right, height);
std::vector<std::vector<glm::vec3>> interpolated;
for (int x = 0; x <= height; x++){
    interpolated.push_back(interpolate(left.at(x),right.at(x),width));
}
return interpolated;

}

void line(DrawingWindow window, CanvasPoint to, CanvasPoint from, Colour c){
  float xDiff = to.x - from.x;
  float yDiff = to.y - from.y;
  int numberOfSteps =  ceil(std::max(abs(xDiff), abs(yDiff)));
  vector<int> X = interpolate(from.x,to.x, numberOfSteps);
  vector<int> Y = interpolate(from.y,to.y, numberOfSteps);
  for (int i = 0; i < numberOfSteps; i++)  window.setPixelColour(X.at(i), Y.at(i), c.pack());
}

void texturedLine(DrawingWindow window, CanvasPoint to, CanvasPoint from, vector<vector<uint32_t>> texture){
  float xDiff = to.x - from.x;
  float yDiff = to.y - from.y;
  int numberOfSteps =  ceil(std::max(abs(xDiff), abs(yDiff)));
  vector<int> X = interpolate(from.x,to.x, numberOfSteps);
  vector<int> Y = interpolate(from.y,to.y, numberOfSteps);
  TexturePoint tStep = to.texturePoint - from.texturePoint;
  for (int i = 0; i < numberOfSteps; i++){
    TexturePoint t = from.texturePoint + (i * tStep/numberOfSteps);
    window.setPixelColour(X.at(i), Y.at(i), texture.at(t.y).at(t.x));
  }  
}

void stroked(DrawingWindow window, CanvasPoint first, CanvasPoint second, CanvasPoint third, Colour c){
  line(window,first,second,c);
  line(window,second,third,c);
  line(window,first,third,c);
}

void filled(DrawingWindow window, CanvasPoint first, CanvasPoint second, CanvasPoint third, Colour c){
  if (first.y < second.y) std::swap(first,second);
  if (first.y < third.y ) std::swap(first,third );
  if (second.y < third.y) std::swap(second,third); 
  //First = top, Second = Middle, Third = Bottom;
  //Next find the intersection of first->third and y=second
  float scale = (first.y-second.y)/(first.y-third.y);
  CanvasPoint extra = CanvasPoint(first - scale*(first-third));

  //Interpolate X values for each Y value
  vector<CanvasPoint> firstToExtra = interpolate(first,extra,ceil(first.y-second.y)+1);
  vector<CanvasPoint> firstToSecond = interpolate(first,second,ceil(first.y-second.y)+1);
  vector<CanvasPoint> thirdToExtra = interpolate(third,extra,ceil(second.y-third.y)+1);
  vector<CanvasPoint> thirdToSecond = interpolate(third,second,ceil(second.y-third.y)+1);

  for (int i = 0; i <= first.y - second.y + 1; i++){
    line(window,firstToExtra[i],firstToSecond[i],c);
  }
  for (int i = 0; i <= second.y - third.y; i++){
    line(window,thirdToExtra[i],thirdToSecond[i],c);
  }
  stroked(window,first,second,third,c);
}

void texturedTriangle(DrawingWindow window, vector<vector<uint32_t>> image, CanvasPoint first, CanvasPoint second, CanvasPoint third){
  // first = CanvasPoint(160,10);
  first.texturePoint = TexturePoint(195,5);
  // second = CanvasPoint(300,230);
  second.texturePoint = TexturePoint(395,380);
  // third = CanvasPoint(10,150);
  third.texturePoint = TexturePoint(65,330);

  if (first.y < second.y) std::swap(first,second);
  if (first.y < third.y ) std::swap(first,third );
  if (second.y < third.y) std::swap(second,third); 

  float scale = (first.y-second.y)/(first.y-third.y);
  CanvasPoint extra = CanvasPoint(first - scale*(first-third));

  vector<CanvasPoint> firstToExtra = interpolate(first,extra,ceil(first.y-second.y)+1);
  vector<CanvasPoint> firstToSecond = interpolate(first,second,ceil(first.y-second.y)+1);
  vector<CanvasPoint> thirdToExtra = interpolate(third,extra,ceil(second.y-third.y)+1);
  vector<CanvasPoint> thirdToSecond = interpolate(third,second,ceil(second.y-third.y)+1);

  for (int i = 0; i <= ceil(first.y - second.y) +1; i++){
    texturedLine(window,firstToExtra[i],firstToSecond[i],image);
  }
  for (int i = 0; i <= ceil(second.y - third.y)+1; i++){
    texturedLine(window,thirdToExtra[i],thirdToSecond[i],image);
  }   
}

vector<vector<uint32_t>> readPPM(const char * filename){
  std::ifstream f;
  std::string line;
  vector<vector<uint32_t>> image;
  f.open( filename, std::ios::binary);
  if (!f.is_open()){
    std::cout << "Failed to open ppm" << std::endl;
    return image;
  }
  while (getline(f,line)) if (line[0] == 'P' and line[1] == '6') break;
  std::cout << "Read P6" << std::endl;
  while (getline(f,line)) if (line[0] != '#') break;
  int width  = stoi(line.substr(0,line.find(' ')));
  int height = stoi(line.substr(line.find(' ')));
  std::cout << width << std::endl;
  std::cout << height << std::endl;
  while (getline(f,line)) if (line[0] != '#') break;
  int maxval = stoi(line);
  std::cout << maxval << std::endl;
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

std::unordered_map<std::string,Colour> readMTL(const char* filename){
  std::ifstream f;
  std::string line;
  std::string name;
  std::unordered_map<std::string,Colour> materials;
  f.open( filename, std::ios::in);
  while (getline(f,line)) {
    if (line.find("newmtl") != std::string::npos) {
      name = line.substr(line.find(' ')+1);
    }
    if (line.find("Kd") != std::string::npos){
      std::string* c = split(line,' ');
      int r = round(255 * stof(c[1]));
      int g = round(255 * stof(c[2]));
      int b = round(255 * stof(c[3]));
      materials[name] = Colour(r,g,b);
      } 
    }
  return materials;
}

vector<ModelTriangle> readOBJ(const char* filename,std::unordered_map<std::string,Colour> mtls, float scale){
  vector<ModelTriangle> triangles;
  vector<glm::vec3> points;
  std::ifstream f;
  std::string line;
  Colour current_colour = Colour(255,255,255);
  f.open(filename, std::ios::in);
  while (getline(f,line)) {
    if (line.find("usemtl") != std::string::npos){
      std::string material = split(line, ' ')[1];
      if (!(mtls.find(material) == mtls.end())) current_colour =  mtls[material];
    }
    else if (line[0] == 'v') {
      std::string* toks = split(line,' ');
      // Z is made negative so image is on positive side of Z, flip y aswell 
      points.push_back(glm::vec3(stof(toks[1])*scale,stof(toks[2])*-scale,stof(toks[3])*-scale));
    }
    else if (line[0] == 'f') {
      std::string* toks = split(line,' ');
      glm::vec3 first = points.at(abs(stoi(split(toks[1],'/')[0])-1));
      glm::vec3 second = points.at(abs(stoi(split(toks[2],'/')[0])-1)); 
      glm::vec3 third = points.at(abs(stoi(split(toks[3],'/')[0])-1));
      ModelTriangle triangle = ModelTriangle(first,second,third,current_colour);
      triangles.push_back(triangle);
    }
  }

  return triangles; 
}