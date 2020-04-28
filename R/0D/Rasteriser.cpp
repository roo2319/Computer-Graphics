#include "Rasteriser.h"

CanvasPoint project(glm::vec3 point, Camera camera, int width, int height)
{
  glm::vec3 d = camera.rotation * (point - camera.position);
  int x = round(camera.focal * (d.x / d.z));
  int y = round(camera.focal * (d.y / d.z));
  CanvasPoint projected = CanvasPoint(x + width / 2, height / 2 - y, 1 / d.z);
  //passing depth of vertices
  return projected;
}

std::vector<CanvasPoint> clip(DrawingWindow window, Camera camera, std::vector<glm::vec3> &points)
{
  std::vector<CanvasPoint> projected;
  std::vector<Plane> frustum = camera.getFrustum();
  for (uint i = 0; i < frustum.size(); i++)
  {
      // glm::vec3 point = camera.rotation * (points[j] - camera.position);
    vector<glm::vec3> out;
    vector<glm::vec3> in;
    for (uint j=0; j < 3; j++)
    {
      if (frustum[i].distance(points[j]) <  0){
        out.push_back(points[j]);
      // std::cout << "Clipped at " << i << " DOT: " <<  frustum[i].distance(points[j]) << std::endl;
      }
      else{
        in.push_back(points[j]);
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
        std::replace(points.begin(),points.end(),out[k],new1);
      }
    }
    if (out.size() == 1){
      std::vector<glm::vec3> newPoints;
      for( int k = 0; k<2; k++){
        glm::vec3 np = glm::normalize(in[k] - out[0]);
        float scale = frustum[i].distance(out[0])  / glm::dot(frustum[i].normal,np);
        glm::vec3 new1 = out[0] - (scale * np);
        newPoints.push_back(new1);
      }
      //Recursion time
      vector<glm::vec3> left = {in[0], in[1], newPoints[1]};
      vector<glm::vec3> right = {newPoints[0],newPoints[1], in[0]};
      vector<CanvasPoint> Pleft = clip(window,camera,left);
      vector<CanvasPoint> Pright = clip(window,camera,right);
      Pleft.insert(Pleft.end(),Pright.begin(),Pright.end());
      return Pleft;

    }
  }
  projected.push_back(project(points[0], camera, window.width, window.height));
  projected.push_back(project(points[1], camera, window.width, window.height));
  projected.push_back(project(points[2], camera, window.width, window.height));
  return projected;
}


void drawWireframe(std::vector<ModelTriangle> model, DrawingWindow window, Camera camera)
{
  camera.updateFrustum(window.width,window.height);
  #pragma omp parallel for
  for (unsigned int i = 0; i < model.size(); i++)
  {
    std::vector<glm::vec3> vertices = {model[i].vertices[0], model[i].vertices[1], model[i].vertices[2]};
    std::vector<CanvasPoint> projections = clip(window, camera, vertices);
    if (projections.size() != 0)
    {
      for(uint k=0; k<projections.size(); k+=3){
        stroked(window, projections[k], projections[k+1], projections[k+2], model[i].colour);
      }
    }
  }
}

void drawRasterised(std::vector<ModelTriangle> model, DrawingWindow window, Camera camera, vector<vector<uint32_t>> image)
{
  camera.updateFrustum(window.width,window.height);
  #pragma omp parallel for
  for (unsigned int i = 0; i < model.size(); i++)
  {
    std::vector<glm::vec3> vertices = {model[i].vertices[0], model[i].vertices[1], model[i].vertices[2]};
    std::vector<CanvasPoint> projections = clip(window, camera, vertices);
    if (projections.size() != 0)
    {
      for(uint k=0; k<projections.size(); k+=3)
      {

        if(model[i].isTexture)
        {
          // std::cout << std::endl<< "nameTexture " << model[i].nameTexture <<std::endl<< std::endl;
          projections[k].texturePoint  = model[i].texture[0];
          projections[k+1].texturePoint = model[i].texture[1];
          projections[k+2].texturePoint  = model[i].texture[2];
          // filled(window, projections[k], projections[k+1], projections[k+2], model[i].colour);
          texturedTriangle(window,image,projections[k], projections[k+1], projections[k+2]);
        }
        else{
          filled(window, projections[k], projections[k+1], projections[k+2], model[i].colour);

        }
      }
    }
  }
}
