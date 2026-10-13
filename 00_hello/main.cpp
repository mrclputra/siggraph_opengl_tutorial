#include <iostream>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

// if this file builds and runs, then vcpkg and cmake are set up correctly

int main() {
   std::cout << "hello world" << "\n";
   std::cout << "glfw: " << glfwGetVersionString() << "\n";

   // glm header only
   glm::vec3 a(2.0f, 4.0f, 2.0f);
   glm::vec3 b(4.0f, 4.0f, 4.0f);
   glm::vec3 sum = a + b;
   std::cout << sum.x << ", " << sum.y << ", " << sum.z << "\n";

   return 0;
}
