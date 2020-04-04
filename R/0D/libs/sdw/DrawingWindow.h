#pragma once

#include "SDL.h"
#include <iostream>

class DrawingWindow
{

private:
  SDL_Window *window;
  SDL_Renderer *renderer;
  SDL_Texture *texture;
  uint32_t *pixelBuffer;
  float *depthBuffer;

public:
  int height;
  int width;

  // Constructor method
  DrawingWindow();
  DrawingWindow(int w, int h, bool fullscreen);
  void destroy();
  void renderFrame();
  bool pollForInputEvents(SDL_Event *event);
  void setPixelColour(int x, int y, uint32_t colour);
  void setDepth(int x, int y, float depth);
  uint32_t getPixelColour(int x, int y);
  float getDepth(int x, int y);
  void clearPixels();
  void clearDepth();

  void printMessageAndQuit(const char* message, const char* error)
  {
    if(error == NULL) {
      std::cout << message << std::endl;
      exit(0);
    }
    else {
      std::cout << message << " " << error <<std::endl;
      exit(1);
    }
  }

};
