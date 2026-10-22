#pragma once

#include <print>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#include <glm/glm.hpp>

// obj has no per-vertex color (that lives in .mtl), so we'll set color in the shader
struct Vertex {
   glm::vec3 pos;
};

// reads positions ("v") and triangulated faces ("f"), skips everything else
bool loadObj(const std::string &path, std::vector<Vertex> &vertices, std::vector<unsigned int> &indices) {
   std::ifstream file(path);
   if (!file) {
      std::println("error, could not open file {}", path);
      return false;
   }

   std::string line;
   while (std::getline(file, line)) {
      std::stringstream ss(line);
      std::string type;
      ss >> type;

      if (type == "v") {
         Vertex vertex;
         ss >> vertex.pos.x >> vertex.pos.y >> vertex.pos.z;
         vertices.push_back(vertex);
      } else if (type == "f") {
         // corners can look like "1", "1/2", "1//3" or "1/2/3" -- std::stoi stops at '/'
         for (int i = 0; i < 3; i++) {
            std::string corner;
            ss >> corner;
            indices.push_back(std::stoi(corner) - 1);  // obj counts from 1
         }
      }
   }

   std::println("loaded {}: {} vertices, {} triangles", path, vertices.size(), indices.size() / 3);
   return true;
}
