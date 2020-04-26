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
    for (uint j=0; j < 3; j++)
    {
      glm::vec3 point = camera.rotation * (points[j] - camera.position);
      if (frustum[i].distance(points[j]) < 0){
      std::cout << "Clipped at " << i << " DOT: " <<  frustum[i].distance(points[j]) << std::endl;
        return projected;
      }
    }
  }
  projected.push_back(project(points[0], camera, window.width, window.height));
  projected.push_back(project(points[1], camera, window.width, window.height));
  projected.push_back(project(points[2], camera, window.width, window.height));
  return projected;
}
//just checks if point in inside image plane
bool inPlane(CanvasPoint points[3], int width, int height)
{
  for (int i = 0; i < 3; i++)
  {
    if (points[i].x < 0 || points[i].x > width)
      return false;
    if (points[i].y < 0 || points[i].y > height)
      return false;
  }
  return true;
}

void drawWireframe(std::vector<ModelTriangle> model, DrawingWindow window, Camera camera)
{
  //Image plane = 0,0,0
  CanvasPoint first, second, third;
  camera.updateFrustum(window.width,window.height);
  for (unsigned int i = 0; i < model.size(); i++)
  {
    std::vector<glm::vec3> vertices = {model[i].vertices[0], model[i].vertices[1], model[i].vertices[2]};
    std::vector<CanvasPoint> projections = clip(window, camera, vertices);
    if (projections.size() != 0)
    {
      stroked(window, projections[0], projections[1], projections[2], model[i].colour);
    }
  }
}

void drawRasterised(std::vector<ModelTriangle> model, DrawingWindow window, Camera camera)
{
  //Image plane = 0,0,0
  CanvasPoint first, second, third;
  camera.updateFrustum(window.width,window.height);
  for (unsigned int i = 0; i < model.size(); i++)
  {
    std::vector<glm::vec3> vertices = {model[i].vertices[0], model[i].vertices[1], model[i].vertices[2]};
    std::vector<CanvasPoint> projections = clip(window, camera, vertices);
    if (projections.size() != 0)
    {
      filled(window, projections[0], projections[1], projections[2], model[i].colour);
    }
  }
}
