#define GLFW_INCLUDE_NONE
#include <glad/glad.h>
#include <GLFW/glfw3.h>

const char *vertex_src = R"(
  #version 330 core
  layout (location = 0) in vec3 vpos;
  layout (location = 1) in vec3 vcolor;  // new, the second attribute

  out vec3 acolor;  // sent to the fragment shader, the name has to match the in there

  void main() {
     gl_Position = vec4(vpos, 1.0);
     acolor = vcolor;
  }
  )";

const char *fragment_src = R"(
  #version 330 core
  in vec3 acolor;  // from the vertex shader, blended between the 3 vertices for every pixel
  out vec4 color;

  void main() {
     color = vec4(acolor, 1.0);
  }
  )";

int main() {
   glfwInit();
   glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
   glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
   glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

   GLFWwindow *window = glfwCreateWindow(1280, 720, "03b_colors", nullptr, nullptr);
   glfwMakeContextCurrent(window);
   gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

   // compile shaders and link into a single program
   unsigned int vertex_shader = glCreateShader(GL_VERTEX_SHADER);
   glShaderSource(vertex_shader, 1, &vertex_src, nullptr);
   glCompileShader(vertex_shader);

   unsigned int fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
   glShaderSource(fragment_shader, 1, &fragment_src, nullptr);
   glCompileShader(fragment_shader);

   unsigned int program = glCreateProgram();
   glAttachShader(program, vertex_shader);
   glAttachShader(program, fragment_shader);
   glLinkProgram(program);

   // each vertex now has a position and a color, 6 floats in total
   // clang-format off
   float vertices[] = {
   //  x      y      z      r     g     b
      -0.5f, -0.5f,  0.0f,  1.0f, 0.0f, 0.0f,  // bottom left, red
       0.5f, -0.5f,  0.0f,  0.0f, 1.0f, 0.0f,  // bottom right, green
       0.0f,  0.5f,  0.0f,  0.0f, 0.0f, 1.0f,  // top, blue
   };
   // clang-format on

   // vao and vbo, see 03_triangle
   unsigned int vao, vbo;
   glGenVertexArrays(1, &vao);
   glGenBuffers(1, &vbo);

   glBindVertexArray(vao);
   glBindBuffer(GL_ARRAY_BUFFER, vbo);
   glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

   // in memory our vertices now look like this
   //
   //   | x  y  z  r  g  b | x  y  z  r  g  b | x  y  z  r  g  b |
   //   |    vertex 0      |    vertex 1      |    vertex 2      |
   //
   // so we need one glVertexAttribPointer call per attribute,
   //    the stride is the same for both, the offset is where each one starts inside a vertex

   // position, location 0
   //    3 floats; stride 6 floats; start from the beginning
   glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
   glEnableVertexAttribArray(0);

   // color, location 1
   //    3 floats; stride 6 floats; starts after the 3 position floats
   glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)(3 * sizeof(float)));
   glEnableVertexAttribArray(1);

   // main loop
   while (!glfwWindowShouldClose(window)) {
      glClearColor(0.16f, 0.21f, 0.14f, 1.0f);
      glClear(GL_COLOR_BUFFER_BIT);

      glUseProgram(program);
      glBindVertexArray(vao);
      glDrawArrays(GL_TRIANGLES, 0, 3);

      glfwSwapBuffers(window);
      glfwPollEvents();
   }

   glfwTerminate();
   return 0;
}
