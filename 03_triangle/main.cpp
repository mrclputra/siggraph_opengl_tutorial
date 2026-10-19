#include <iostream>

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
     color = vec4(1.0, 0.5, 0.2, 1.0);
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

   // here we define our scene data
   //    in this case we want to render a single triangle
   // clang-format off
   float vertices[] = {
   //  x      y      z
      -0.5f, -0.5f,  0.0f,  // bottom left
       0.5f, -0.5f,  0.0f,  // bottom right
       0.0f,  0.5f,  0.0f,  // top
   };
   // clang-format on

   // remember! the gpu is a separate device!
   // our vertices are in cpu memory right now, which means the gpu can't see them yet
   //    remember! the gpu is a separate device!
   //       - we need to copy them over into gpu memory (vram)
   //       - we also neet to tell the gpu how to read them
   //
   // vbo (vertex buffer object) is the actual memory on the gpu, it holds our raw vertex data
   //    the gpu doesn't know what's inside, to it this is just a bunch of bytes
   // vao (vertex array object) remembers how to read those bytes--
   //    which buffer to use, how many floats per vertex, where each attribute starts
   //    so when drawing we just bind the vao and everything is set up again

   // opengl does not give us objects, it gives us ids that refers to things on the gpu
   //    you will see the use of unsigned ints a lot in graphics programming
   unsigned int vao, vbo;
   glGenVertexArrays(1, &vao);
   glGenBuffers(1, &vbo);

   // binding basically means "this is the thing i am working with now"
   //    every call after this affects whatever is bound; opengl works like a big state machine
   // bind the vao first, so it records everything we set up below
   glBindVertexArray(vao);
   glBindBuffer(GL_ARRAY_BUFFER, vbo);

   // copy our vertices into the vbo
   //    GL_STATIC_DRAW is a hint that we upload once and draw many times, so the driver can put it somewhere fast
   //    there are many other flags we can use but for the sake of simplicity we use this one
   glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

   // now we describe the layout, so the gpu knows what the bytes mean
   //    0                  -> attribute location, more on this next
   //    3, GL_FLOAT        -> each vertex has 3 floats (x, y, z)
   //    GL_FALSE           -> don't normalize, use the values as they are
   //    3 * sizeof(float)  -> stride, how many bytes to jump to get to the next vertex
   //    (void *)0          -> offset, where this attribute starts inside a vertex, here at the very beginning
   glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
   // https://registry.khronos.org/OpenGL-Refpages/gl4/html/glVertexAttribPointer.xhtml

   // attributes are disabled by default, turn this one on
   glEnableVertexAttribArray(0);

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
