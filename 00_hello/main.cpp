#include <print>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <glm/glm.hpp>

// if this file builds and runs, then vcpkg and cmake are set up correctly

int main() {
   std::println("hello world");
   std::println("glfw: {}", glfwGetVersionString());

   // test glm
   glm::vec3 a(2.0f, 4.0f, 2.0f);
   glm::vec3 b(4.0f, 4.0f, 4.0f);
   glm::vec3 sum = a + b;
   std::println("{}, {}, {}", sum.x, sum.y, sum.z);

   // test glfw + glad
   // glad needs an opengl context, which needs a window, hidden
   glfwInit();
   glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
   glfwMakeContextCurrent(glfwCreateWindow(100, 100, "", nullptr, nullptr));
   gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
   std::println("opengl: {}.{}", GLVersion.major, GLVersion.minor);  // 0.0 means glad failed
   glfwTerminate();

   // dont worry if you dont understand the code above,
   // we will go into detail in the following chapters

   return 0;
}
