#define GLFW_INCLUDE_NONE
#include <glad/glad.h>
#include <GLFW/glfw3.h>

const char *vertex_src = R"(
  #version 330 core
  layout (location = 0) in vec3 vpos;
  layout (location = 1) in vec3 vcolor;  // new, second attribute

  out vec3 acolor;  // sent to fragment shader, name has to match

  void main() {
     gl_Position = vec4(vpos, 1.0);
     acolor = vcolor;
  }
  )";

const char *fragment_src = R"(
  #version 330 core
  in vec3 acolor;  // interpolated across the triangle for each pixel
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

   // each vertex now has position + color, 6 floats total
   // clang-format off
   float vertices[] = {
   //  x      y      z      r     g     b
      -0.5f, -0.5f,  0.0f,  1.0f, 0.0f, 0.0f,  // bottom left, red
       0.5f, -0.5f,  0.0f,  0.0f, 1.0f, 0.0f,  // bottom right, green
       0.0f,  0.5f,  0.0f,  0.0f, 0.0f, 1.0f,  // top, blue
   };
   // clang-format on

   unsigned int vao, vbo;
   glGenVertexArrays(1, &vao);
   glGenBuffers(1, &vbo);

   glBindVertexArray(vao);
   glBindBuffer(GL_ARRAY_BUFFER, vbo);
   glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

   // in memory: | x y z r g b | x y z r g b | x y z r g b |
   // one glVertexAttribPointer per attribute, same stride, different offset

   // position, location 0: 3 floats, stride 6, offset 0
   glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
   glEnableVertexAttribArray(0);

   // color, location 1: 3 floats, stride 6, offset 3
   glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)(3 * sizeof(float)));
   glEnableVertexAttribArray(1);

   while (!glfwWindowShouldClose(window)) {
      glClearColor(0.17f, 0.22f, 0.24f, 1.0f);
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
