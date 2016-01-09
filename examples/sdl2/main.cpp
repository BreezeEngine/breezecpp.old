#include <SDL_video.h>
#include <SDL/SDL.h>
#include <glad/glad.h>
#include <iostream>
void
RunGame(SDL_Window* mainWindow)
{
  bool loop = true;
  while (loop) {
    std::cout << SDL_GetError() << '\n';
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
    std::cout << SDL_GetError() << '\n';
     // if (event.type == SDL_QUIT)
     //   loop = false;

     // if (event.type == SDL_KEYDOWN) {
     //   switch (event.key.keysym.sym) {
     //     case SDLK_ESCAPE:
     //       loop = false;
     //       break;
     //     case SDLK_r:
     //       // Cover with red and update
     //       glClearColor(1.0, 0.0, 0.0, 1.0);
     //       glClear(GL_COLOR_BUFFER_BIT);
     //       SDL_GL_SwapWindow(mainWindow);
     //       break;
     //     case SDLK_g:
     //       // Cover with green and update
     //       glClearColor(0.0, 1.0, 0.0, 1.0);
     //       glClear(GL_COLOR_BUFFER_BIT);
     //       SDL_GL_SwapWindow(mainWindow);
     //       break;
     //     case SDLK_b:
     //       // Cover with blue and update
     //       glClearColor(0.0, 0.0, 1.0, 1.0);
     //       glClear(GL_COLOR_BUFFER_BIT);
     //       SDL_GL_SwapWindow(mainWindow);
     //       break;
     //     default:
     //       break;
     //   }
     // }
    }

    // Swap our back buffer to the front
    // This is the same as :
    // 		SDL_RenderPresent(&renderer);
  }
}

int
main()
{
  SDL_Init(SDL_INIT_VIDEO); // Initialize SDL2

  SDL_Window* window; // Declare a pointer to an SDL_Window
  SDL_GLContext context;

  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
  // Create an application window with the following settings:
  window = SDL_CreateWindow(
    "An SDL2 window",        //    const char* title
    SDL_WINDOWPOS_UNDEFINED, //    int x: initial x position
    SDL_WINDOWPOS_UNDEFINED, //    int y: initial y position
    640,                     //    int w: width, in pixels
    480,                     //    int h: height, in pixels
    SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN         //    Uint32 flags: window options, see docs
    );
  context = SDL_GL_CreateContext(window);
  gladLoadGL();
  // Check that the window was successfully made
  if (window == NULL) {
    // In the event that the window could not be made...
    std::cout << "Could not create window: " << SDL_GetError() << '\n';
    SDL_Quit();
    return 1;
  }
  std::cout << SDL_GetError() << '\n';
  /* This makes our buffer swap syncronized with the monitor's vertical refresh */
  SDL_GL_SetSwapInterval(1);

  /* Swap our back buffer to the front */
  RunGame(window);
  // The window is open: enter program loop (see SDL_PollEvent)
  // Close and destroy the window
  SDL_DestroyWindow(window);

  // Clean up SDL2 and exit the program
  SDL_Quit();
  return 0;
}

//#include <stdio.h>
//#include <stdlib.h>
///* If using gl3.h */
///* Ensure we are using opengl's core profile only */
//#define GL3_PROTOTYPES 1
//#include <glad/glad.h>
//
//#include <SDL.h>
//#define PROGRAM_NAME "Tutorial1"
//
///* A simple function that prints a message, the error code returned by SDL,
// * and quits the application */
//void sdldie(const char *msg)
//{
//    printf("%s: %s\n", msg, SDL_GetError());
//    SDL_Quit();
//    exit(1);
//}
//
//
//void checkSDLError(int line = -1)
//{
//#ifndef NDEBUG
//  const char *error = SDL_GetError();
//  if (*error != '\0')
//  {
//    printf("SDL Error: %s\n", error);
//    if (line != -1)
//      printf(" + line: %i\n", line);
//    SDL_ClearError();
//  }
//#endif
//}
//
//
///* Our program's entry point */
//int main(int argc, char *argv[])
//{
//    SDL_Window *mainwindow; /* Our window handle */
//    SDL_GLContext maincontext; /* Our opengl context handle */
//
//    if (SDL_Init(SDL_INIT_VIDEO) < 0) /* Initialize SDL's Video subsystem */
//        sdldie("Unable to initialize SDL"); /* Or die on error */
//
//    /* Request opengl 3.2 context.
//     * SDL doesn't have the ability to choose which profile at this time of writing,
//     * but it should default to the core profile */
//    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
//    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
//
//    /* Turn on double buffering with a 24bit Z buffer.
//     * You may need to change this to 16 or 32 for your system */
//    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
//    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
//
//    /* Create our window centered at 512x512 resolution */
//    mainwindow = SDL_CreateWindow(PROGRAM_NAME, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
//        512, 512, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
//    if (!mainwindow) /* Die if creation failed */
//        sdldie("Unable to create window");
//
//    checkSDLError(__LINE__);
//
//    /* Create our opengl context and attach it to our window */
//    gladLoadGL();
//    maincontext = SDL_GL_CreateContext(mainwindow);
//    checkSDLError(__LINE__);
//
//
//    /* This makes our buffer swap syncronized with the monitor's vertical refresh */
//    SDL_GL_SetSwapInterval(1);
//
//    /* Clear our buffer with a red background */
//    glClearColor ( 1.0, 0.0, 0.0, 1.0 );
//    glClear ( GL_COLOR_BUFFER_BIT );
//    /* Swap our back buffer to the front */
//    SDL_GL_SwapWindow(mainwindow);
//    /* Wait 2 seconds */
//    SDL_Delay(2000);
//
//    /* Same as above, but green */
//    glClearColor ( 0.0, 1.0, 0.0, 1.0 );
//    glClear ( GL_COLOR_BUFFER_BIT );
//    SDL_GL_SwapWindow(mainwindow);
//    SDL_Delay(2000);
//
//    /* Same as above, but blue */
//    glClearColor ( 0.0, 0.0, 1.0, 1.0 );
//    glClear ( GL_COLOR_BUFFER_BIT );
//    SDL_GL_SwapWindow(mainwindow);
//    SDL_Delay(2000);
//
//    /* Delete our opengl context, destroy our window, and shutdown SDL */
//    SDL_GL_DeleteContext(maincontext);
//    SDL_DestroyWindow(mainwindow);
//    SDL_Quit();
//
//    return 0;
//}
//
