#include "Utils.h"

int ComputeOutCode(CanvasPoint p,DrawingWindow w){
  int code = 0;

  if (p.x < 0) code |= 1;
  else if (p.x > w.width-1) code |= 2;
  if (p.y < 0) code |= 4;
  else if (p.y > w.height-1) code |= 8;
  return code;
}

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
vector<int> interpolate(float from, float to, int numberOfValues)
{
  vector<int> interpolated;
  for (int i = 0; i <= numberOfValues; i++){
      interpolated.push_back(round(from + i*((to-from)/numberOfValues)));
  }
  return interpolated;
}

vector<CanvasPoint> interpolate(CanvasPoint from, CanvasPoint to, int numberOfValues)
{
  vector<CanvasPoint> interpolated;
  for (int i = 0; i <= numberOfValues; i++){
      interpolated.push_back(from + (i * (to - from)/numberOfValues));
  }
  return interpolated;
}

vector<glm::vec3> interpolate(glm::vec3 from, glm::vec3 to, int numberOfValues)
{
  vector<glm::vec3> interpolated;
  glm::vec3 step = ((float)1/numberOfValues) * (to - from);
  for (int  i = 0; i <= numberOfValues; i++){
      interpolated.push_back(from + (float(i) * step));
  }
  return interpolated;
}

vector<vector<glm::vec3>> interpolate2d(glm::vec3 top_left, glm::vec3 top_right, glm::vec3 bottom_left, glm::vec3 bottom_right, int width, int height)
{
  vector<glm::vec3> left = interpolate(top_left,bottom_left,height);
  vector<glm::vec3> right = interpolate(top_right, bottom_right, height);
  vector<vector<glm::vec3>> interpolated;
  for (int x = 0; x <= height; x++){
      interpolated.push_back(interpolate(left.at(x),right.at(x),width));
  }
  return interpolated;

}

// Bresenhams
void bresenham(DrawingWindow window, CanvasPoint to, CanvasPoint from, Colour c){
  int x0, x1, y0, y1;
  float d0, d1;
  x0 = from.x; y0 = from.y; x1 = to.x; y1 = to.y; d0 = from.depth; d1=to.depth;
  bool steep = abs(y1-y0) > abs(x1-x0);
  if (steep){
    std::swap(x0,y0);
    std::swap(x1,y1);
  }
  
  if (x0 > x1){
    std::swap(x0,x1);
    std::swap(y0,y1);
    std::swap(d0,d1);
  }

  int dx = x1 - x0;
  int dy = abs(y1 - y0);
  // depth per step
  double dps = (d1 - d0)/dx;
  double d = d0;

  int error = dx/2;
  int ystep = (y0 < y1) ? 1 : -1;
  //Carefully consider rounding
  int y = floor(y0); 
  int maxX = ceil(x1);

  for (int x = floor(x0); x <= maxX; x++){
    if (steep) window.setPixelColourDC(y,x,d,c.pack());
    else window.setPixelColourDC(x,y,d,c.pack());
    d += dps;
    error -= dy;
    if (error<0){
      y += ystep;
      error += dx;
    }
  } 





}
void line(DrawingWindow window, CanvasPoint to, CanvasPoint from, Colour c){
  // Cohen-Sutherland clip then bresenham
  // std::cout <<"plotting line from " << to << " to " << from << std::endl; 
  int outcode0 = ComputeOutCode(from,window);
  int outcode1 = ComputeOutCode(to,window);
  bool accept = false;

  while (true){
    if(!(outcode0|outcode1)){
      accept = true; 
      break;
    }
    else if (outcode0&outcode1) {
      break;
    }
    else{
      CanvasPoint p;
      int outcodeOut = outcode1 > outcode0 ? outcode1:outcode0;
      double dy = to.y - from.y;
      double dx = to.x - from.x;
      double dd = to.depth - from.depth;

      // Watch out for overflows

      if (outcodeOut&8){
        p = CanvasPoint(from.x + (window.height-1-from.y)*(dx/dy),
                        window.height-1,
                        from.depth + (window.height-1-from.y)*(dd/dy));
      }

      else if(outcodeOut&4){
        p = CanvasPoint(from.x +  (-from.y)*(dx/dy),
                        0,
                        from.depth + (-from.y)*(dd/dy));
      }
      else if(outcodeOut&2){
        p = CanvasPoint(window.width-1,
                       from.y+(window.width-1-from.x)*(dy/dx),
                       from.depth +(window.width-1-from.x)*(dd/dx));
      }
      else if(outcodeOut&1){
        p = CanvasPoint(0,
                       from.y+(-from.x)*(dy/dx),
                       from.depth +(-from.x)*(dd/dx));

      }
      if (outcodeOut==outcode0){
        from = CanvasPoint(p.x,p.y,p.depth);
        outcode0 = ComputeOutCode(from,window);
      }
      else{
        to = CanvasPoint(p.x,p.y,p.depth);
        outcode1 = ComputeOutCode(to,window);
      }
    }
  }

  if (accept){
    // std::cout <<"Clipped to " << to << " to " << from << std::endl; 
    bresenham(window,to,from,c);
  }
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
      row.push_back(c.pack());
    }
    image.push_back(row);
  }
  f.close();
  return image;
}

void writePPM(const char * filename,DrawingWindow window){
  std::ofstream f;
  std::string line;
  f.open( filename, std::ios::binary);
  if (!f.is_open()){
    std::cout << "Failed to open ppm" << std::endl;
  }
  f << "P6" << std::endl;
  f << window.width << " " << window.height << std::endl;
  f << "255" << std::endl;
  for(int y = 0; y < window.height; y++){
    for (int x = 0; x < window.width; x++){
      Colour pixel = Colour(window.getPixelColour(x,y));
      f.write((const char*)(&pixel.red),1 );
      f.write((const char*)(&pixel.green),1 );
      f.write((const char*)(&pixel.blue),1 );
    }
  }

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
      materials[name] = Colour(name,r,g,b);
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
      points.push_back(glm::vec3(stof(toks[1])*scale,stof(toks[2])*scale,stof(toks[3])*-scale));
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

void redNoise(DrawingWindow window){
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

void drawppm(DrawingWindow window){
  vector<vector<uint32_t>> image = readPPM("texture.ppm");
  for(unsigned int y = 0; y < image.size(); y++){
    for (unsigned int x = 0; x < image.at(0).size(); x++){
      window.setPixelColour(x,y,image.at(y).at(x));
    }
  }
}

void greyscale(DrawingWindow window){
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

void four_colour(DrawingWindow window){
  vector<vector<glm::vec3>> colour = interpolate2d(glm::vec3(255,0,0),glm::vec3(0,0,255),glm::vec3(255,255,0),glm::vec3(0,255,0),window.width,window.height);
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
