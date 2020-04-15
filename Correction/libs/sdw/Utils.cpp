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
void line(DrawingWindow window, CanvasPoint to, CanvasPoint from, Colour c){
  float x0, x1, y0, y1, d0, d1;
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

  float dx = x1 - x0;
  float dy = abs(y1 - y0);
  // depth per step
  float dps = (d1 - d0)/dx;
  float d = d0;

  float error = dx/2.0f;
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
  guide(window, first, second, third, extra);

}
void guide(DrawingWindow window, CanvasPoint first, CanvasPoint second, CanvasPoint third, CanvasPoint extra)
{
  stroked(window,CanvasPoint(first.x,first.y),CanvasPoint(second.x,second.y),CanvasPoint(third.x,third.y),Colour(0,255,255));
  line (window, CanvasPoint(extra.x,extra.y),CanvasPoint(second.x,second.y),Colour(255,0,255));
  // star(window, first,Colour(255,0,0));
  // star(window, second,Colour(255,255,0));
  // star(window, third,Colour(0,0,255));
  // star(window, extra,Colour(255,0,255));
}
void star(DrawingWindow window, CanvasPoint star, Colour c)
{
  line (window, CanvasPoint(star.x-6,star.y),CanvasPoint(star.x+5,star.y),c);
  line (window, CanvasPoint(star.x,star.y-6),CanvasPoint(star.x,star.y+5),c);
  line (window, CanvasPoint(star.x-5,star.y-5),CanvasPoint(star.x+4,star.y+4),c);
  line (window, CanvasPoint(star.x-5,star.y+5),CanvasPoint(star.x+4,star.y-4),c);

}
int findSteps(CanvasPoint from, CanvasPoint to)
{
  float xDiff = to.x - from.x;
  float yDiff = to.y - from.y;
  int numberOfSteps =  ceil(std::max(abs(xDiff), abs(yDiff)));
  return numberOfSteps;
}












void texturedLine(DrawingWindow window, CanvasPoint to, CanvasPoint from, vector<vector<uint32_t>> texture){
  float xDiff = to.x - from.x;
  float yDiff = to.y - from.y;
  int numberOfSteps =  ceil(std::max(abs(xDiff), abs(yDiff)));

  vector<CanvasPoint> interpolated = interpolateT(from,to,numberOfSteps, 0);
  CanvasPoint Pt ;TexturePoint t;
  // TexturePoint tStep = to.texturePoint - from.texturePoint;

  for (int i = 0; i < numberOfSteps; i++)
  {
    Pt = interpolated[i];
    t = Pt.texturePoint;
    //t = from.texturePoint + (i * tStep/numberOfSteps);
    // window.setPixelColour(std::ceil(Pt.x), std::ceil(Pt.y), texture.at(std::round(t.y)).at(std::round(t.x)));
    window.setPixelColour((Pt.x), (Pt.y), texture.at(t.y).at(t.x));
  }
}

void texturedTriangle(DrawingWindow window, vector<vector<uint32_t>> image, CanvasPoint first, CanvasPoint second, CanvasPoint third, int triNo)
{
  if (first.y > second.y) std::swap(first,second);
  if (first.y > third.y ) std::swap(first,third );
  if (second.y > third.y) std::swap(second,third);
  if (first.y == second.y && first.x > second.x) std::swap(first,second);
  if (second.y == third.y && second.x > third.x) std::swap(second,third);

  float scale = (second.y-first.y)/(third.y-first.y);
  CanvasPoint extra = CanvasPoint(first + scale*(third-first));
  extra = findTexture(first, third, extra);

  vector<CanvasPoint> firstToExtra = interpolateT(first,extra,ceil(second.y - first.y)+1, triNo);
  vector<CanvasPoint> firstToSecond = interpolateT(first,second,ceil(second.y - first.y)+1, triNo);
  vector<CanvasPoint> thirdToExtra = interpolateT(third,extra,ceil(third.y - second.y)+1, triNo);
  vector<CanvasPoint> thirdToSecond = interpolateT(third,second,ceil(third.y - second.y)+1, triNo);

  for (int i = 0; i <= ceil(second.y - first.y) +1; i++){
    texturedLine(window,firstToExtra[i],firstToSecond[i],image);
  }
  for (int i = 0; i <= ceil(third.y - second.y) +1; i++){
    texturedLine(window,thirdToExtra[i],thirdToSecond[i],image);
  }
  guide(window, first, second, third, extra);
}

vector<float> interpolateF(float from, float to, int numberOfValues)
{
  vector<float> interpolated;
  for (float i = 0.0f; i <= (float)numberOfValues; i++){
      interpolated.push_back(round(from + i*((to-from)/(float)numberOfValues)));
  }
  return interpolated;
}
vector<double> interpolateDepth(double from, double to, int numberOfValues)
{
  vector<double> interpolated;
  for (double i = 0.0; i <= (double)numberOfValues; i++){
      interpolated.push_back((from + i*((to-from)/(double)numberOfValues)));
  }
  return interpolated;
}
vector<CanvasPoint> interpolateT(CanvasPoint from, CanvasPoint to, int numberOfValues,  int triNo)
{
  vector<CanvasPoint> interpolated;
  vector<float> X         = interpolateF(from.x,      to.x,     numberOfValues);
  vector<float> Y         = interpolateF(from.y,     to.y,     numberOfValues);
  vector<double> depth    = interpolateDepth(from.depth, to.depth, numberOfValues);
  CanvasPoint temp;
  for (int i = 0; i <= numberOfValues; i++)
  {
      temp = CanvasPoint(X[i], Y[i], depth[i]);
      temp = findTexture(from, to, temp);
      interpolated.push_back(temp);
  }
  return interpolated;
}

CanvasPoint findTexture(CanvasPoint from, CanvasPoint to, CanvasPoint P)
{
  float z0, z1, c0, c1, currentDist, totalDist, q , cx, cy;
  //if(from.depth < to.depth) std::swap(from, to);

  currentDist = std::sqrt( std::pow((P .x - from.x), 2) + std::pow((P .y - from.y), 2) /*+ std::pow((P .depth - from.depth), 2) */);
  totalDist   = std::sqrt( std::pow((to.x - from.x), 2) + std::pow((to.y - from.y), 2) /*+ std::pow((to .depth - from.depth), 2) */);
  q =  (currentDist / totalDist);
  z0 = from.depth;
  z1 = to.depth;


  c0 = from.texturePoint.x;
  c1 = to.texturePoint.x;
  cx = (  ((c0*(1-q))/z0)+((c1*q)/z1)  )  /  (  ((1-q)/z0)+(q/z1)  );
  c0 = from.texturePoint.y;
  c1 = to.texturePoint.y;
  q = currentDist/totalDist ;
  cy = (  ((c0*(1-q))/z0)+((c1*q)/z1)  )  /  (  ((1-q)/z0)+(q/z1)  );
  cx = round(abs(cx)); cy = round(abs(cy));

  if((from.x == to.x && from.y == to.y) || (from.x == P.x && from.y == P.y)){
    P = from;
    P.texturePoint = from.texturePoint;
  }
  else if(P.x == to.x && P.y == to.y ){
    P = to;
    P.texturePoint = to.texturePoint;
  }
  else{
    P.texturePoint = TexturePoint(cx, cy);
  }

  cx = P.texturePoint.x;  cy =P.texturePoint.y;
  if( cx < 0.0 || cy < 0.0 || cx > 501.0 || cy > 501.0 || std::isnan(cx) || std::isnan(cy)){
    std::cout << std::endl << from << P << to ;
    std::cout <<"("<< cx <<"\t, " << cy <<")" ;
    std::cout << "Q = " << q;
    std::cout << std::endl  ;
  }

  return P;
}













/*
//chekerboard
vector<vector<uint32_t>> readPPM(const char * filename){
  std::ifstream f;
  std::string line;
  vector<vector<uint32_t>> image;
  f.open( filename, std::ios::binary);
  if (!f.is_open()){
    std::cout << "Failed to open ppm" << std::endl;
    return image;
  }

  int width,height,maxval;
  if (getline(f,line))
  {
    if(line[0] == 'P' and line[1] == '6'){
      //line = line.substr(2);
      std::cout << std::endl << "Read P6" << std::endl;
    }
    width  = stoi(line.substr(3,line.find(' ')));
    std::cout << width << std::endl;
    height = stoi(line.substr(7,line.find(' ')));
    std::cout << height << std::endl;
    maxval = stoi(line.substr(line.find(' ')));
    std::cout << maxval << std::endl;
  }
  for(int y = 0; y < height; y++){
    vector<uint32_t> row;
    for (int x = 0; x < width; x++){

      Colour c;
      c.red   = f.get();
      c.green = f.get();
      c.blue  = f.get();
      //std::cout << c << std::endl;
      row.push_back(c.pack());
    }
    image.push_back(row);
  }
  f.close();
  std::cout << "PPM read success" << std::endl << std::endl;
  return image;
}
*/

//texture
//chekerboard2
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
