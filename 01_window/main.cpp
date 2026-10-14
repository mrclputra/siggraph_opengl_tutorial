#include <iostream>

// glfw is our windowing library, it creates the window + opengl context, and can handle input
//    it does not draw anything, which is what opengl is for
// GLFW_INCLUDE_NONE tells glfw not to include any opengl headers of its own, glad does that for us
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

// glad loads the opengl functions from the graphics driver, more on this below
#include <glad/glad.h>

// this function is called by GLFW whenever something goes wrong
void errorCallback(int error, const char *description) {
   std::cout << "glfw error " << error << ": " << description << "\n";
}

// this function is called by GLFW whenever the window is resized
void framebufferSizeCallback(GLFWwindow *, int width, int height) {
   // tell opengl to resize the viewport accordingly
   glViewport(0, 0, width, height);
}

int main() {
   std::cout << "01 GLFW Window Example" << "\n";

   // set the glfw error callback
   glfwSetErrorCallback(errorCallback);

   // initialize GLFW
   if (!glfwInit()) {
      return 1;  // fail
   }

   // ask for an OpenGL 3.3 core context
   glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
   glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
   glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

   // create the window, this also creates its OpenGL context
   GLFWwindow *window = glfwCreateWindow(1280, 720, "01_window", nullptr, nullptr);
   if (window == nullptr) {
      glfwTerminate();
      return 1;
   }

   // make the window's context the one OpenGL calls go to
   glfwMakeContextCurrent(window);

   // load the OpenGL functions
   //
   // OpenGL is not a library you link against like GLFW. it is a specification,
   // and the actual functions (glClear, glViewport, ...) are implemented by your
   // graphics driver (NVIDIA, AMD, Intel, ...). their addresses are only known
   // at runtime, after a context exists, so they have to be looked up one by one.
   //
   // every os has its own way of looking these up, wglGetProcAddress on windows, glXGetProcAddress on linux and so on
   // glfwGetProcAddress handles that for us, you can give it something like "glClear" and it gives you back the address
   //
   // glad then calls that for every opengl function and saves the addresses,
   //    after this we can just call glClear() and stuff like normal
   // the cast is there because the types don't match exactly--glad wants a void* back and glfw returns a function pointer,
   //    but they're both just addresses so it works fine
   //
   // note that all this must happen after glfwMakeContextCurrent;
   // without a current context, there is nothing to load from.
   if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
      std::cout << "failed to load OpenGL\n";
      glfwTerminate();
      return 1;
   }

   // set callback for window resizing
   glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

   // main loop, runs once every frame until the window is closed
   while (!glfwWindowShouldClose(window)) {
      // clear the screen to a color
      glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
      glClear(GL_COLOR_BUFFER_BIT);

      glfwSwapBuffers(window);  // show what we drew
      glfwPollEvents();         // handle input and window events
   }

   // clean up
   glfwDestroyWindow(window);
   glfwTerminate();
   return 0;
}
