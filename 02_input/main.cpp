#include <iostream>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>

void errorCallback(int error, const char *description) {
   std::cout << "glfw error " << error << ": " << description << "\n";
}
void framebufferSizeCallback(GLFWwindow *, int width, int height) {
   glViewport(0, 0, width, height);
}

// polling: we call this ourselves every frame to check the current state of the mouse
void pollMouse(GLFWwindow *window) {
   double x, y;
   glfwGetCursorPos(window, &x, &y);  // in pixels, from the top left of the window
   // std::cout << "mouse: " << x << ", " << y << "\n";
}

// callback: glfw calls this once whenever a key is pressed, held or released
void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods) {
   if (action != GLFW_PRESS) return;  // only react to the initial press
   if (key == GLFW_KEY_ESCAPE) glfwSetWindowShouldClose(window, true);
   if (key == GLFW_KEY_SPACE) std::cout << "space pressed" << "\n";
}

int main() {
   std::cout << "02 GLFW Input Example" << "\n";

   glfwSetErrorCallback(errorCallback);

   if (!glfwInit()) {
      return 1;  // fail
   }

   // opengl 3.3 core
   glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
   glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
   glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

   // create the window and its opengl context
   GLFWwindow *window = glfwCreateWindow(1280, 720, "02_input", nullptr, nullptr);
   if (window == nullptr) {
      glfwTerminate();
      return 1;
   }
   glfwMakeContextCurrent(window);

   // load opengl functions
   if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
      std::cout << "failed to load OpenGL\n";
      glfwTerminate();
      return 1;
   }

   glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
   glfwSetKeyCallback(window, keyCallback);

   // main loop
   while (!glfwWindowShouldClose(window)) {
      pollMouse(window);

      glClearColor(0.16f, 0.21f, 0.14f, 1.0f);
      glClear(GL_COLOR_BUFFER_BIT);

      glfwSwapBuffers(window);
      glfwPollEvents();
   }

   glfwDestroyWindow(window);
   glfwTerminate();
   return 0;
}
