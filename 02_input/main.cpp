#include <iostream>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>

void glfw_error_callback(int error, const char *description) {
   std::cout << "glfw error " << error << ": " << description << "\n";
}
void glfw_framebuffer_size_callback(GLFWwindow *, int width, int height) {
   glViewport(0, 0, width, height);
}

// polling: we call this ourselves every frame to check the current state of the mouse
void glfw_poll_mouse(GLFWwindow *window) {
   double x, y;
   glfwGetCursorPos(window, &x, &y);  // in pixels, from the top left of the window
   // std::cout << "mouse: " << x << ", " << y << "\n";
}

// callback: glfw calls this once whenever a key is pressed, held or released
void glfw_key_callback(GLFWwindow *window, int key, int scancode, int action, int mods) {
   if (action != GLFW_PRESS) return;  // only react to the initial press
   if (key == GLFW_KEY_ESCAPE) glfwSetWindowShouldClose(window, true);
   if (key == GLFW_KEY_SPACE) std::cout << "space pressed" << "\n";
}

int main() {
   std::cout << "02 GLFW Input Example" << "\n";

   glfwSetErrorCallback(glfw_error_callback);

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

   // load opengl functions, see 01_window for details
   if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
      std::cout << "failed to load OpenGL\n";
      glfwTerminate();
      return 1;
   }

   glfwSetFramebufferSizeCallback(window, glfw_framebuffer_size_callback);
   glfwSetKeyCallback(window, glfw_key_callback);

   // main loop
   while (!glfwWindowShouldClose(window)) {
      glfw_poll_mouse(window);

      glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
      glClear(GL_COLOR_BUFFER_BIT);

      glfwSwapBuffers(window);
      glfwPollEvents();
   }

   glfwDestroyWindow(window);
   glfwTerminate();
   return 0;
}
