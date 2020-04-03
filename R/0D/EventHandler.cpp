#include "EventHandler.h"

int counter = 1;

void handleEvent(SDL_Event event, DrawingWindow window, Camera& camera, vector<vector<uint32_t>> image, int& renderer)
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
      camera.updateRotation(0.2,0,0);
    }  
    else if(event.key.keysym.sym == SDLK_DOWN) {
      std::cout << "DOWN" << std::endl;
      camera.updateRotation(-0.2,0,0);
    }  

    else if(event.key.keysym.sym == SDLK_LSHIFT){
      std::cout << "Q" << std::endl;
      camera.updateRotation(0,0,-0.2);    
      }
    else if(event.key.keysym.sym == SDLK_RSHIFT){
      std::cout << "Q" << std::endl;
      camera.updateRotation(0,0,0.2);    
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
      camera.up(1);
    }
    else if(event.key.keysym.sym == SDLK_e){
      std::cout << "moving camera down" << std::endl;
      camera.up(-1);
    }
    else if(event.key.keysym.sym == SDLK_a){
      std::cout << "moving camera left" << std::endl;
      camera.right(-1);
    }
    else if(event.key.keysym.sym == SDLK_d){
      std::cout << "moving camera right" << std::endl;
      camera.right(1);
    }
    else if(event.key.keysym.sym == SDLK_w){
      std::cout << "moving camera forward" << std::endl;
      camera.forward(1);
    }
    else if(event.key.keysym.sym == SDLK_s){
      std::cout << "moving camera backward" << std::endl;
      camera.forward(-1);
    }
    // l for look :)
    else if(event.key.keysym.sym == SDLK_l){
      std::cout << "looking at point" << std::endl;
      camera.lookat(glm::vec3(0,2,1));
    }
    else if(event.key.keysym.sym == SDLK_p){
      std::cout << "Camera at " << camera.position.x << " " << camera.position.y << " " << camera.position.z << std::endl;
    }
    else if(event.key.keysym.sym == SDLK_n){
      renderer = (renderer + 1) % 3;
    }

    else if(event.key.keysym.sym == SDLK_RETURN){
      std::cout << "Writing to file" << std::endl;
      char filename[50];
      sprintf(filename,"captures/%d.ppm",counter);
      writePPM(filename, window);
      counter++;
      std::cout << "Done" << std::endl;
    }
  }
  else if(event.type == SDL_MOUSEBUTTONDOWN) std::cout << "MOUSE CLICKED" << std::endl;
  else if(event.type == SDL_QUIT){
	exit(1);
  }
}
