#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include <glad/glad.h>
#include <glm/glm.hpp>

// i have personally reused this class in many of my opengl projects
// i suggest you do something similar; write something once and reuse it again and again

class Shader {
 public:
   Shader(const std::string &vertex_path, const std::string &fragment_path) {
      // build shader program instantly
      unsigned int vertex = compile(GL_VERTEX_SHADER, readFile(vertex_path));
      unsigned int fragment = compile(GL_FRAGMENT_SHADER, readFile(fragment_path));

      // a program is the vertex and fragment shader linked together,
      //    this is what we draw with
      program = glCreateProgram();
      glAttachShader(program, vertex);
      glAttachShader(program, fragment);
      glLinkProgram(program);

      // check if linking worked
      GLint ok = GL_FALSE;
      glGetProgramiv(program, GL_LINK_STATUS, &ok);
      if (!ok) {
         GLchar info_log[1024];
         glGetProgramInfoLog(program, 1024, nullptr, info_log);
         std::cout << "SHADER_LINK_ERROR: " << info_log << "\n";
         glDeleteProgram(program);
         program = 0;  // 0 means "no program", binding it just draws nothing
      } else {
         std::cout << "linked shader program " << program << "\n";
      }

      // since we already have the program, we dont need these anymore
      glDeleteShader(vertex);
      glDeleteShader(fragment);
   }

   void bind() {
      glUseProgram(program);
   }
   void unbind() {
      glUseProgram(0);
   }

   // uniforms
   //    glGetUniformLocation finds the uniform by its name in the shader, then we set its value
   //    the program must be bound first
   void setMat4(const std::string &name, const glm::mat4 &mat) {
      glUniformMatrix4fv(glGetUniformLocation(program, name.c_str()), 1, GL_FALSE, &mat[0][0]);
   }
   void setMat3(const std::string &name, const glm::mat3 &mat) {
      glUniformMatrix3fv(glGetUniformLocation(program, name.c_str()), 1, GL_FALSE, &mat[0][0]);
   }
   void setMat2(const std::string &name, const glm::mat2 &mat) {
      glUniformMatrix2fv(glGetUniformLocation(program, name.c_str()), 1, GL_FALSE, &mat[0][0]);
   }
   void setVec4(const std::string &name, const glm::vec4 &vec) {
      glUniform4fv(glGetUniformLocation(program, name.c_str()), 1, &vec[0]);
   }
   void setVec3(const std::string &name, const glm::vec3 &vec) {
      glUniform3fv(glGetUniformLocation(program, name.c_str()), 1, &vec[0]);
   }
   void setInt(const std::string &name, const int value) {
      glUniform1i(glGetUniformLocation(program, name.c_str()), value);
   }
   void setUInt(const std::string &name, const unsigned int value) {
      glUniform1ui(glGetUniformLocation(program, name.c_str()), value);
   }
   void setFloat(const std::string &name, const float value) {
      glUniform1f(glGetUniformLocation(program, name.c_str()), value);
   }

 private:
   GLuint program = 0;  // shader program
   static GLuint compile(GLenum type, const std::string &source) {
      GLuint shader = glCreateShader(type);

      const char *c = source.c_str();
      glShaderSource(shader, 1, &c, nullptr);
      glCompileShader(shader);

      // check if it compiled
      GLint ok = GL_FALSE;
      glGetShaderiv(shader, GL_COMPILE_STATUS, &ok);
      if (!ok) {
         GLchar info_log[1024];
         glGetShaderInfoLog(shader, 1024, nullptr, info_log);
         std::cout << "SHADER_COMPILE_ERROR: " << info_log << "\n";
         glDeleteShader(shader);
         return 0;
      }
      std::cout << "compile shader source: " << shader << "\n";
      return shader;
   }

   // read the whole file into a string
   static std::string readFile(const std::string &path) {
      std::ifstream file(path);
      if (!file) std::cout << "error, could not open file " << path << "\n";
      std::stringstream ss;
      ss << file.rdbuf();
      return ss.str();
   }
};
