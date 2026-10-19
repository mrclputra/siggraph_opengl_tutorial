#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include "shader.h"

int main() {
   glfwInit();
   glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
   glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
   glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

   GLFWwindow *window = glfwCreateWindow(1280, 720, "05_ebo", nullptr, nullptr);
   glfwMakeContextCurrent(window);
   gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

   Shader shader(SHADER_DIR "shaders/vertex.glsl", SHADER_DIR "shaders/fragment.glsl");

   // notice how we only have 4 vertices
   // clang-format off
   float vertices[] = {
   //  x      y      z      r     g     b
      -0.5f, -0.5f,  0.0f,  1.0f, 0.0f, 0.0f,  // 0 bottom left
       0.5f, -0.5f,  0.0f,  0.0f, 1.0f, 0.0f,  // 1 bottom right
       0.5f,  0.5f,  0.0f,  0.0f, 0.0f, 1.0f,  // 2 top right
      -0.5f,  0.5f,  0.0f,  1.0f, 1.0f, 0.0f,  // 3 top left
   };

   // a quad is 2 triangles, and they share the corners 0 and 2
   //
   //   3 ---------- 2
   //   |          / |
   //   |  2nd   /   |
   //   |      /     |
   //   |    /   1st |
   //   |  /         |
   //   0 ---------- 1
   //
   // without an ebo we would need 6 vertices, with 0 and 2 stored twice
   // with an ebo we store each corner once, and the indices say which corners make each triangle
   //    when we draw, we dont iterate over the vao anymore, but rather the ebo
   //    the gpu reads the indices 3 at a time and looks up the vertex with that number
   //    see how vertex 0 and 2 are shared?
   unsigned int indices[] = {
      0, 1, 2,  // first triangle
      2, 3, 0,  // second triangle
   };
   // clang-format on

   // initialize buffers
   unsigned int vao, vbo, ebo;
   glGenVertexArrays(1, &vao);
   glGenBuffers(1, &vbo);
   glGenBuffers(1, &ebo);

   glBindVertexArray(vao);
   glBindBuffer(GL_ARRAY_BUFFER, vbo);
   glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

   // the vao also remembers the ebo,
   //    bind it while the vao is bound
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
   glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

   glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
   glEnableVertexAttribArray(0);
   glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)(3 * sizeof(float)));
   glEnableVertexAttribArray(1);

   while (!glfwWindowShouldClose(window)) {
      glClearColor(0.16f, 0.21f, 0.14f, 1.0f);
      glClear(GL_COLOR_BUFFER_BIT);

      shader.bind();

      // draw using ebo, instead of going through vertices in order
      // 6 indices, of type unsigned int
      glBindVertexArray(vao);
      glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

      glfwSwapBuffers(window);
      glfwPollEvents();
   }

   glfwTerminate();
   return 0;
}
