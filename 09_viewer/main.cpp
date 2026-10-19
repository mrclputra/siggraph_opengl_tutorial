#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>

#include <algorithm>
#include <cstddef>
#include <vector>

#include "shader.h"
#include "loader.h"
#include "camera.h"
#include "input.h"

// called by glfw whenever the window is resized, see 01_window
void framebufferSizeCallback(GLFWwindow *, int width, int height) {
   glViewport(0, 0, width, height);
}

int main() {
   glfwInit();
   glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
   glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
   glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

   GLFWwindow *window = glfwCreateWindow(1280, 720, "09_viewer", nullptr, nullptr);
   glfwMakeContextCurrent(window);
   gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

   glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

   Shader shader(SHADER_DIR "shaders/vertex.glsl", SHADER_DIR "shaders/fragment.glsl");

   glEnable(GL_DEPTH_TEST);

   std::vector<Vertex> vertices;
   std::vector<unsigned int> indices;
   if (!loadObj(SHADER_DIR "dragon.obj", vertices, indices) || vertices.empty()) {
      glfwTerminate();
      return 1;
   }

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
   glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, normal));
   glEnableVertexAttribArray(1);

   Input input(window);

   // find the box around the model, so the camera can start looking at the middle of it from far enough away
   //    models can be any size and anywhere, so we can't just hardcode the camera like before
   glm::vec3 bbox_min = vertices[0].pos;
   glm::vec3 bbox_max = vertices[0].pos;
   for (const Vertex &vertex : vertices) {
      bbox_min = glm::min(bbox_min, vertex.pos);
      bbox_max = glm::max(bbox_max, vertex.pos);
   }
   Camera camera;
   camera.frame((bbox_min + bbox_max) * 0.5f, glm::length(bbox_max - bbox_min) * 0.5f);

   while (!glfwWindowShouldClose(window)) {
      // the window can be resized now, so we ask for its size every frame instead of using 1280x720
      //    a minimized window is 0 pixels high, std::max keeps us from dividing by 0 in the projection
      int width, height;
      glfwGetFramebufferSize(window, &width, &height);
      height = std::max(height, 1);

      // controls
      //    left mouse to orbit, middle mouse to pan, scroll to zoom, escape to quit
      if (input.isKeyDown(GLFW_KEY_ESCAPE))
         glfwSetWindowShouldClose(window, true);
      if (input.isMouseDown(GLFW_MOUSE_BUTTON_LEFT)) {
         glm::vec2 curr = glm::vec2(input.cursor_x, input.cursor_y);
         glm::vec2 prev = curr - glm::vec2(input.cursor_dx, input.cursor_dy);
         camera.orbit(prev, curr, width, height);
      }
      if (input.isMouseDown(GLFW_MOUSE_BUTTON_MIDDLE))
         camera.pan(float(input.cursor_dx), float(input.cursor_dy));
      if (input.scroll_delta != 0.0)
         camera.zoom(float(input.scroll_delta));
      input.update();

      glClearColor(0.17f, 0.22f, 0.24f, 1.0f);
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

      shader.bind();

      // the model stays still now, the camera moves instead
      shader.setMat4("model", glm::mat4(1.0f));
      shader.setMat4("view", camera.getViewMatrix());
      shader.setMat4("projection", camera.getProjMatrix(width, height));

      // the light sits on the camera like a headlight, so whatever we look at is lit
      shader.setVec3("light_pos", camera.position);
      shader.setVec3("view_pos", camera.position);

      glBindVertexArray(vao);
      glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);

      glfwSwapBuffers(window);
      glfwPollEvents();
   }

   glfwTerminate();
   return 0;
}
