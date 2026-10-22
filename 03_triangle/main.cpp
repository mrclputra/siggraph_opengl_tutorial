#define GLFW_INCLUDE_NONE
#include <glad/glad.h>
#include <GLFW/glfw3.h>
// #include <glm/glm.hpp>

const char *vertex_src = R"(
  #version 330 core
  layout (location = 0) in vec3 vpos;
  void main() {
     gl_Position = vec4(vpos, 1.0);
  }
  )";

const char *fragment_src = R"(
  #version 330 core
  out vec4 color;
  void main() {
     // color = vec4(1.0, 0.5, 0.2, 1.0);
     color = vec4(1.0, 1.0, 1.0, 1.0)
  }
  )";

int main() {
   glfwInit();
   glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
   glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
   glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

   GLFWwindow *window = glfwCreateWindow(1280, 720, "03_triangle", nullptr, nullptr);
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

   // clang-format off
   float vertices[] = {
   //  x      y      z
      -0.5f, -0.5f,  0.0f,  // bottom left
       0.5f, -0.5f,  0.0f,  // bottom right
       0.0f,  0.5f,  0.0f,  // top
   };
   // clang-format on

   // the gpu is a separate device -- vertices are in cpu memory, the gpu can't see them yet
   // vbo holds the raw bytes on the gpu; vao remembers how to read them (buffer, stride, attributes)
   unsigned int vao, vbo;
   glGenVertexArrays(1, &vao);
   glGenBuffers(1, &vbo);

   // bind vao first so it records everything we set up below
   // binding means "this is what i'm working with" -- opengl is a big state machine
   glBindVertexArray(vao);
   glBindBuffer(GL_ARRAY_BUFFER, vbo);

   // copy vertices to gpu; GL_STATIC_DRAW hints upload-once draw-many
   glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

   // describe the layout so the gpu knows what the bytes mean
   //    0                 -> attribute location
   //    3, GL_FLOAT       -> 3 floats per vertex
   //    GL_FALSE          -> don't normalize
   //    3 * sizeof(float) -> stride (bytes to next vertex)
   //    (void*)0          -> offset (where this attribute starts)
   glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
   // https://registry.khronos.org/OpenGL-Refpages/gl4/html/glVertexAttribPointer.xhtml

   glEnableVertexAttribArray(0);  // attributes are disabled by default

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
