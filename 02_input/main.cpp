#include <print>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>

void errorCallback(int error, const char *description) {
   std::println("glfw error {}: {}", error, description);
}
void framebufferSizeCallback(GLFWwindow *, int width, int height) {
   glViewport(0, 0, width, height);
}

// polling: we call this ourselves every frame to read the current mouse position
void pollMouse(GLFWwindow *window) {
   double x, y;
   glfwGetCursorPos(window, &x, &y);  // in pixels, from the top left of the window
   // std::println("mouse: {}, {}", x, y);
}

// callback: glfw calls this once whenever a key is pressed, held, or released
void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods) {
   if (action != GLFW_PRESS) return;  // only react to the initial press
   if (key == GLFW_KEY_ESCAPE) glfwSetWindowShouldClose(window, true);
   if (key == GLFW_KEY_SPACE) std::println("space pressed");
}

int main() {
   std::println("02 GLFW Input Example");

   glfwSetErrorCallback(errorCallback);

   if (!glfwInit()) {
      return 1;  // fail
   }

   glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
   glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
   glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

   GLFWwindow *window = glfwCreateWindow(1280, 720, "02_input", nullptr, nullptr);
   if (window == nullptr) {
      glfwTerminate();
      return 1;  // fail
   }
   glfwMakeContextCurrent(window);

   if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
      std::println("failed to load OpenGL");
      glfwTerminate();
      return 1;  // fail
   }

   glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
   glfwSetKeyCallback(window, keyCallback);

   while (!glfwWindowShouldClose(window)) {
      pollMouse(window);

      glClearColor(0.17f, 0.22f, 0.24f, 1.0f);
      glClear(GL_COLOR_BUFFER_BIT);

      glfwSwapBuffers(window);
      glfwPollEvents();
   }

   glfwDestroyWindow(window);
   glfwTerminate();
   return 0;
}
