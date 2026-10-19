#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>

#include "shader.h"

int main() {
   glfwInit();
   glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
   glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
   glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

   GLFWwindow *window = glfwCreateWindow(1280, 720, "06_transforms", nullptr, nullptr);
   glfwMakeContextCurrent(window);
   gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

   Shader shader(SHADER_DIR "shaders/vertex.glsl", SHADER_DIR "shaders/fragment.glsl");

   // clang-format off
   float vertices[] = {
   //  x      y      z      r     g     b
      -0.5f, -0.5f,  0.0f,  1.0f, 0.0f, 0.0f,  // 0 bottom left
       0.5f, -0.5f,  0.0f,  0.0f, 1.0f, 0.0f,  // 1 bottom right
       0.5f,  0.5f,  0.0f,  0.0f, 0.0f, 1.0f,  // 2 top right
      -0.5f,  0.5f,  0.0f,  1.0f, 1.0f, 0.0f,  // 3 top left
   };

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

      // most people use a system of 3 matrices for putting vertices on a screen
      //    glm::mat4(1.0f) is an identity matrix, a matrix that does nothing, we will build on top of it

      // this is the model matrix
      //    where the object is in the "world"
      //    here i spin it over time around an axis
      glm::mat4 model = glm::rotate(glm::mat4(1.0f), (float)glfwGetTime(), glm::vec3(0.5f, 1.0f, 0.0f));

      // this is the view matrix
      //    where the camera is (how the object is positioned relative to the camera)
      //    remember the right-hand-y-up rule!
      glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -2.0f));

      // this is the projection matrix
      //    defines where stuff goes on the screen
      //    requires an fov, screen dimensions, and a near/far plane
      glm::mat4 projection = glm::perspective(glm::radians(45.0f), 1280.0f / 720.0f, 0.1f, 100.0f);

      shader.setMat4("model", model);
      shader.setMat4("view", view);
      shader.setMat4("projection", projection);

      glBindVertexArray(vao);
      glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

      glfwSwapBuffers(window);
      glfwPollEvents();
   }

   glfwTerminate();
   return 0;
}
