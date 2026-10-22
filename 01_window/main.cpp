#include <print>

// glfw creates the window and opengl context, handles input
// GLFW_INCLUDE_NONE: let glad include the opengl headers instead of glfw
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

// glad loads the opengl function pointers from the graphics driver at runtime
#include <glad/glad.h>

void errorCallback(int error, const char *description) {
   std::println("glfw error {}: {}", error, description);
}

void framebufferSizeCallback(GLFWwindow *, int width, int height) {
   glViewport(0, 0, width, height);
}

int main() {
   std::println("01 GLFW Window Example");

   glfwSetErrorCallback(errorCallback);

   if (!glfwInit()) {
      return 1;  // fail
   }

   // ask for opengl 3.3 core
   glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
   glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
   glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

   GLFWwindow *window = glfwCreateWindow(1280, 720, "01_window", nullptr, nullptr);
   if (window == nullptr) {
      glfwTerminate();
      return 1;  // fail
   }

   glfwMakeContextCurrent(window);

   // opengl is not a library we link against -- the functions live in the driver and
   // have to be loaded at runtime. glad does this for us using glfwGetProcAddress
   if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
      std::println("failed to load OpenGL");
      glfwTerminate();
      return 1;  // fail
   }

   glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

   while (!glfwWindowShouldClose(window)) {
      // opengl wants colors from 0 to 1, so this is (44, 56, 61) / 255
      glClearColor(0.17f, 0.22f, 0.24f, 1.0f);
      glClear(GL_COLOR_BUFFER_BIT);

      glfwSwapBuffers(window);  // show what we drew
      glfwPollEvents();         // handle input and window events
   }

   glfwDestroyWindow(window);
   glfwTerminate();
   return 0;
}
