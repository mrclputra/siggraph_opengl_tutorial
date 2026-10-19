#pragma once

#include <print>
#include <fstream>
#include <sstream>
#include <string>

#include <glad/glad.h>
#include <glm/glm.hpp>

// see 04_shader for details
class Shader {
 public:
   Shader(const std::string &vertex_path, const std::string &fragment_path) {
      unsigned int vertex = compile(GL_VERTEX_SHADER, readFile(vertex_path));
      unsigned int fragment = compile(GL_FRAGMENT_SHADER, readFile(fragment_path));

      program = glCreateProgram();
      glAttachShader(program, vertex);
      glAttachShader(program, fragment);
      glLinkProgram(program);

      GLint ok = GL_FALSE;
      glGetProgramiv(program, GL_LINK_STATUS, &ok);
      if (!ok) {
         GLchar info_log[1024];
         glGetProgramInfoLog(program, 1024, nullptr, info_log);
         std::println("SHADER_LINK_ERROR: {}", info_log);
         glDeleteProgram(program);
         program = 0;
      } else {
         std::println("linked shader program {}", program);
      }

      glDeleteShader(vertex);
      glDeleteShader(fragment);
   }

   void bind() {
      glUseProgram(program);
   }
   void unbind() {
      glUseProgram(0);
   }

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
   GLuint program = 0;
   static GLuint compile(GLenum type, const std::string &source) {
      GLuint shader = glCreateShader(type);

      const char *c = source.c_str();
      glShaderSource(shader, 1, &c, nullptr);
      glCompileShader(shader);

      GLint ok = GL_FALSE;
      glGetShaderiv(shader, GL_COMPILE_STATUS, &ok);
      if (!ok) {
         GLchar info_log[1024];
         glGetShaderInfoLog(shader, 1024, nullptr, info_log);
         std::println("SHADER_COMPILE_ERROR: {}", info_log);
         glDeleteShader(shader);
         return 0;
      }
      std::println("compile shader source: {}", shader);
      return shader;
   }

   static std::string readFile(const std::string &path) {
      std::ifstream file(path);
      if (!file) std::println("error, could not open file {}", path);
      std::stringstream ss;
      ss << file.rdbuf();
      return ss.str();
   }
};
