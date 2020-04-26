#include "Camera.h"

Camera::Camera()
{
}

Camera::Camera(glm::vec3 p, glm::mat3 r, float f)
{
    position = p; 
    rotation = r;
    focal    = f;
    near = 1;
    far = 100;
    frustum = {}; 
}  


void Camera::updateRotation(float X, float Y, float Z){
  glm::mat3 xrot = glm::mat3(1,0,0,0,cos(X),sin(X),0,-sin(X),cos(X));
  glm::mat3 yrot = glm::mat3(cos(Y),0,-sin(Y),0,1,0,sin(Y),0,cos(Y));
  glm::mat3 zrot = glm::mat3(cos(Z),sin(Z),0,-sin(Z),cos(Z),0,0,0,1);
  rotation = xrot * yrot * zrot * rotation;
}

void Camera::lookat(glm::vec3 point){
  glm::vec3 forward = glm::normalize(point-position);
  glm::vec3 right   = glm::cross(glm::vec3(0,1,0),forward);
  glm::vec3 up      = glm::cross(forward,right);
  
  rotation[0][0] = right.x; 
  rotation[1][0] = right.y; 
  rotation[2][0] = right.z; 
  rotation[0][1] = up.x; 
  rotation[1][1] = up.y; 
  rotation[2][1] = up.z; 
  rotation[0][2] = forward.x; 
  rotation[1][2] = forward.y; 
  rotation[2][2] = forward.z; 
}

void Camera::updateFrustum(int width,int height){
  std::vector<Plane> f;
  glm::vec3 right = rotation * glm::vec3(1, 0, 0);
  glm::vec3 up = rotation * glm::vec3(0, 1, 0);
  glm::vec3 forward = rotation * glm::vec3(0, 0, 1);

  glm::vec3 fc = position + forward * far;

  glm::vec3 nc = position + forward * near;

  glm::vec3 tmp;

  glm::vec3 NEAR = forward;
  glm::vec3 NEARP = nc;
  f.push_back(Plane(NEAR,NEARP));

  glm::vec3 FAR = -forward;
  glm::vec3 FARP = fc;
  f.push_back(Plane(FAR,FARP));


  tmp = (nc + (up * (height * near / 2))) - position;
  glm::vec3 TOP = glm::normalize(glm::cross(tmp, right));
  glm::vec3 TOPP = (nc - (up * (height * near / 2)));
  // glm::vec3 TOPP = (nc + (up * (height * near / 2)));
  f.push_back(Plane(TOP,TOPP));


  tmp = (nc - (up * (height * near / 2))) - position;
  glm::vec3 BOTTOM = glm::normalize(glm::cross(right, tmp));
  glm::vec3 BOTTOMP = (nc + (up * (height * near / 2)));
  // glm::vec3 BOTTOMP = (nc - (up * (height * near / 2)));
  f.push_back(Plane(BOTTOM,BOTTOMP));


  tmp = (nc + (right * (width * near / 2))) - position;
  glm::vec3 LEFT = glm::normalize(glm::cross(tmp, up));
  glm::vec3 LEFTP = (nc + (right * (width * near / 2)));
  f.push_back(Plane(LEFT,LEFTP));


  tmp = (nc - (right * (width * near / 2))) - position;
  glm::vec3 RIGHT = glm::normalize(glm::cross(up, tmp));
  glm::vec3 RIGHTP = (nc - (right * (width * near / 2)));
  f.push_back(Plane(RIGHT,RIGHTP));

  frustum = f;
}

std::vector<Plane> Camera::getFrustum(){
  return frustum;
}


void Camera::forward(float amount){
  position += (glm::vec3(0,0,amount) * rotation);
}
void Camera::right(float amount){
  position += (glm::vec3(amount,0,0) * rotation);
}
void Camera::up(float amount){
  position += (glm::vec3(0,amount,0) * rotation);
}

Plane::Plane(glm::vec3 n, glm::vec3 p){
  normal = n;
  point = p;
}
float Plane::distance(glm::vec3 p){
  return glm::dot(normal, p-point);
}
