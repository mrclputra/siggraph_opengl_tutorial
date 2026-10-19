#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>

#include <cstddef>
#include <vector>

#include "shader.h"
#include "loader.h"

int main() {
   glfwInit();
   glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
   glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
   glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

   GLFWwindow *window = glfwCreateWindow(1280, 720, "07_loader", nullptr, nullptr);
   glfwMakeContextCurrent(window);
   gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

   Shader shader(SHADER_DIR "shaders/vertex.glsl", SHADER_DIR "shaders/fragment.glsl");

   // a 3d object has things in front of other things, so we need to enable depth testing
   //    the gpu remembers how far away every pixel is, (this is called the depth buffer) and skips the pixels behind
   //    without this, faces will just draw over each other in whatever order they are called
   glEnable(GL_DEPTH_TEST);

   // instead of typing the vertices and indices by hand, we load them from a file
   //    std::vector is an array that can grow, since we don't know the size until we read the file
   //    just be VERY careful with loading the entire file into memory, especially if it is large!! try chunking -marcel
   std::vector<Vertex> vertices;
   std::vector<unsigned int> indices;
   loadObj(SHADER_DIR "cube.obj", vertices, indices);

   // initialize buffers
   unsigned int vao, vbo, ebo;
   glGenVertexArrays(1, &vao);
   glGenBuffers(1, &vbo);
   glGenBuffers(1, &ebo);

   glBindVertexArray(vao);
   glBindBuffer(GL_ARRAY_BUFFER, vbo);
   glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
   glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

   glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, pos));
   glEnableVertexAttribArray(0);
   glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, color));
   glEnableVertexAttribArray(1);

   while (!glfwWindowShouldClose(window)) {
      glClearColor(0.16f, 0.21f, 0.14f, 1.0f);
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  // clear the depth buffer too, every frame

      shader.bind();

      // model, view, projection, see 06_transforms
      glm::mat4 model = glm::rotate(glm::mat4(1.0f), (float)glfwGetTime(), glm::vec3(0.5f, 1.0f, 0.0f));
      glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -3.0f));
      glm::mat4 projection = glm::perspective(glm::radians(45.0f), 1280.0f / 720.0f, 0.1f, 100.0f);

      shader.setMat4("model", model);
      shader.setMat4("view", view);
      shader.setMat4("projection", projection);

      glBindVertexArray(vao);
      glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);

      glfwSwapBuffers(window);
      glfwPollEvents();
   }

   glfwTerminate();
   return 0;
}
