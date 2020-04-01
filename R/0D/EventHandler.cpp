#include "EventHandler.h"

void handleEvent(SDL_Event event, DrawingWindow window, Camera& camera, vector<vector<uint32_t>> image)
{
  if(event.type == SDL_KEYDOWN) {
    if(event.key.keysym.sym == SDLK_LEFT){
      std::cout << "LEFT" << std::endl;
      camera.updateRotation(0,0.2,0);
    }  
    else if(event.key.keysym.sym == SDLK_RIGHT) {
      std::cout << "Right" << std::endl;
      camera.updateRotation(0,-0.2,0);
    }  
    else if(event.key.keysym.sym == SDLK_UP) {
      std::cout << "UP" << std::endl;
      camera.updateRotation(-0.2,0,0);
    }  
    else if(event.key.keysym.sym == SDLK_DOWN) {
      std::cout << "DOWN" << std::endl;
      camera.updateRotation(0.2,0,0);
    }  
    else if(event.key.keysym.sym == SDLK_j) {
      stroked(window, CanvasPoint(rand()%window.width,rand()%window.height),
              CanvasPoint(rand()%window.width,rand()%window.height),
              CanvasPoint(rand()%window.width,rand()%window.height),
              Colour(rand()%255,rand()%255,rand()%255));
      std::cout << "J" << std::endl;
    }
    else if(event.key.keysym.sym == SDLK_f) {
      filled(window,CanvasPoint(rand()%window.width,rand()%window.height),
              CanvasPoint(rand()%window.width,rand()%window.height),
              CanvasPoint(rand()%window.width,rand()%window.height),
              Colour(rand()%255,rand()%255,rand()%255));
      std::cout << "f" << std::endl;
    }
    else if (event.key.keysym.sym == SDLK_t){
      texturedTriangle(window,image,CanvasPoint(rand()%window.width,rand()%window.height),
              CanvasPoint(rand()%window.width,rand()%window.height),
              CanvasPoint(rand()%window.width,rand()%window.height));
      std::cout << "t" << std::endl;

    }
    else if(event.key.keysym.sym == SDLK_c){
        window.clearPixels();
    }
    else if(event.key.keysym.sym == SDLK_q){
      std::cout << "moving camera up" << std::endl;
      camera.position -= glm::vec3(0,1,0) * camera.rotation;
    }
    else if(event.key.keysym.sym == SDLK_e){
      std::cout << "moving camera down" << std::endl;
      camera.position += glm::vec3(0,1,0) * camera.rotation;
    }
    else if(event.key.keysym.sym == SDLK_a){
      std::cout << "moving camera left" << std::endl;
      camera.position -= glm::vec3(1,0,0) * camera.rotation;
    }
    else if(event.key.keysym.sym == SDLK_d){
      std::cout << "moving camera right" << std::endl;
      camera.position += glm::vec3(1,0,0) * camera.rotation;
    }
    else if(event.key.keysym.sym == SDLK_w){
      std::cout << "moving camera forward" << std::endl;
      camera.position += glm::vec3(0,0,1) * camera.rotation  ;
    }
    else if(event.key.keysym.sym == SDLK_s){
      std::cout << "moving camera backward" << std::endl;
      camera.position -= glm::vec3(0,0,1) * camera.rotation;
    }
  }
  else if(event.type == SDL_MOUSEBUTTONDOWN) std::cout << "MOUSE CLICKED" << std::endl;
  else if(event.type == SDL_QUIT){
	exit(1);
  }
}
