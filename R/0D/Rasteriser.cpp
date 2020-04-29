#include "Rasteriser.h"

CanvasPoint project(glm::vec3 point, TexturePoint texture, Camera camera, int width, int height)
{
  glm::vec3 d = camera.rotation * (point - camera.position);
  int x = round(camera.focal * (d.x / d.z));
  int y = round(camera.focal * (d.y / d.z));
  CanvasPoint projected = CanvasPoint(x + width / 2, height / 2 - y, 1 / d.z);
  projected.texturePoint = texture;
  //passing depth of vertices
  if (projected.x<0) projected.x = 0;
  else if (projected.x>width-1) projected.x = width-1;
  if (projected.y<0) projected.y = 0;
  else if (projected.y>height-1) projected.y = height-1;
  return projected;
}

std::vector<CanvasPoint> clip(DrawingWindow window, Camera camera, std::vector<glm::vec3> &points, std::vector<TexturePoint> &textures)
{
  std::vector<CanvasPoint> projected;
  std::vector<Plane> frustum = camera.getFrustum();
  for (uint i = 0; i < frustum.size(); i++)
  {
      // glm::vec3 point = camera.rotation * (points[j] - camera.position);
    vector<glm::vec3> out;
    vector<glm::vec3> in;
    vector<TexturePoint> outTP;
    vector<TexturePoint> inTP;
    TexturePoint new1TP;
    for (uint j=0; j < 3; j++)
    {
      if (frustum[i].distance(points[j]) <  0){
        out.push_back(points[j]);
        outTP.push_back(textures[j]);
      // std::cout << "Clipped at " << i << " DOT: " <<  frustum[i].distance(points[j]) << std::endl;
      }
      else{
        in.push_back(points[j]);
        inTP.push_back(textures[j]);
      }
    }
    // Completely out of range
    if (out.size() == 3){
      return projected;
    }
    if (out.size() == 2){
      for( int k = 0; k<2; k++){
        glm::vec3 np = glm::normalize(in[0] - out[k]);
        float scale = frustum[i].distance(out[k])  / glm::dot(frustum[i].normal,np);
        glm::vec3 new1 = out[k] - (scale * np);
        // std::cout << "This should be 0: (" << frustum[i].distance(new1) << ")" << new1.x <<"," << new1.y <<","<< new1.z << std::endl;
//////////finds new point texture values
        new1TP = linearTexture(in[0], new1, out[k], inTP[0], outTP[k]);
        std::replace(points.begin(),points.end(),out[k],new1);
        for(int i=0 ; i<2;i++){
          if(textures[i]==outTP[k]) textures[i]=new1TP
        }

      }
    }
    if (out.size() == 1){
      std::vector<glm::vec3> newPoints;
      std::vector<TexturePoint> newPointsTP;
      for( int k = 0; k<2; k++){
        glm::vec3 np = glm::normalize(in[k] - out[0]);
        float scale = frustum[i].distance(out[0])  / glm::dot(frustum[i].normal,np);
        glm::vec3 new1 = out[0] - (scale * np);
//////////finds new point texture values
        new1TP = linearTexture(in[k], new1, out[0], inTP[k], outTP[0]);
        newPoints.push_back(new1);
        newPointsTP.push_back(new1TP);
      }
      //Recursion time
      vector<glm::vec3> left = {in[0], in[1], newPoints[1]};
      vector<TexturePoint> leftTP = {inTP[0], inTP[1], newPointsTP[1]};
      vector<glm::vec3> right = {newPoints[0],newPoints[1], in[0]};
      vector<TexturePoint> rightTP = {newPointsTP[0],newPointsTP[1], inTP[0]};
      vector<CanvasPoint> Pleft = clip(window,camera,left,leftTP);
      vector<CanvasPoint> Pright = clip(window,camera,right,rightTP);
      Pleft.insert(Pleft.end(),Pright.begin(),Pright.end());
      PleftTP.insert(PleftTP.end(),PrightTP.begin(),PrightTP.end());
      return Pleft;

    }
  }
  projected.push_back(project(points[0], textures[0], camera, window.width, window.height));
  projected.push_back(project(points[1], textures[1], camera, window.width, window.height));
  projected.push_back(project(points[2], textures[2], camera, window.width, window.height));
  return projected;
}

TexturePoint linearTexture(glm::vec3 from, glm::vec3 P, glm::vec3 to, TexturePoint fromTP, TexturePoint toTP){
  currentDist = std::sqrt( std::pow((P .[0] - from.[0]), 2) + std::pow((P .[1] - from.[1]), 2) );
  totalDist   = std::sqrt( std::pow((to.[0] - from.[0]), 2) + std::pow((to.[1] - from.[1]), 2) );
  int dx = toTp.x - fromTP.x;
  int dy = toTp.y - fromTP.y;
  int Px = fromTP.x + (dx * currentDist / totalDist);
  int Py = fromTP.y + (dy * currentDist / totalDist);
  return TexturePoint(Px, Py);
}

void drawWireframe(std::vector<Model> world, DrawingWindow window, Camera camera)
{
  camera.updateFrustum(window.width,window.height);
  std::vector<ModelTriangle> faces;
  for (uint i = 0; i<world.size(); i++){
    faces.insert(faces.end(),world[i].faces.begin(),world[i].faces.end());
  }
  #pragma omp parallel for
  for(uint j =0; j<faces.size(); j++){
    std::vector<glm::vec3> vertices = {faces[j].vertices[0], faces[j].vertices[1], faces[j].vertices[2]};
    std::vector<TexturePoint> textures = {faces[j].texture[0], faces[j].texture[1], faces[j].texture[2]};
    std::vector<CanvasPoint> projections = clip(window, camera, vertices, textures);
    if (projections.size() != 0)
    {
      for(uint k=0; k<projections.size(); k+=3){
        stroked(window, projections[k], projections[k+1], projections[k+2], faces[j].colour);
      }
    }
  }
}

void drawRasterised(std::vector<Model> world, DrawingWindow window, Camera camera)
{
  camera.updateFrustum(window.width,window.height);
  std::vector<ModelTriangle> faces;
  // for (uint i = 0; i<world.size(); i++){
  //   faces.insert(faces.end(),world[i].faces.begin(),world[i].faces.end());
  // }
  for (uint i = 0; i<world.size(); i++){
    for (uint j = 0; j<world[i].size(); i++){
      if(world[i][j].isTexture){
        world[i][j].image = world[i].image;
      }
      faces.push_back(world[i][j]);
    }
  }

  #pragma omp parallel for
  for(uint j =0; j<faces.size(); j++){
    std::vector<glm::vec3> vertices = {faces[j].vertices[0], faces[j].vertices[1], faces[j].vertices[2]};
    std::vector<TexturePoint> textures = {faces[j].texture[0], faces[j].texture[1], faces[j].texture[2]};
    std::vector<CanvasPoint> projections = clip(window, camera, vertices, textures, faces[j].isTexture);
    if (projections.size() != 0)
    {
      for(uint k=0; k<projections.size(); k+=3)
      {

        if(faces[j].isTexture)
        {
          // std::cout << std::endl<< "nameTexture " << model[i].nameTexture <<std::endl<< std::endl;
          // projections[k].texturePoint  = faces[j].texture[0];
          // projections[k+1].texturePoint = faces[j].texture[1];
          // projections[k+2].texturePoint  = faces[j].texture[2];
          // filled(window, projections[k], projections[k+1], projections[k+2], model[i].colour);
          texturedTriangle(window, faces[j].image,projections[k], projections[k+1], projections[k+2]);
        }
        else{
          filled(window, projections[k], projections[k+1], projections[k+2], faces[j].colour);

        }
      }
    }
  }
}
