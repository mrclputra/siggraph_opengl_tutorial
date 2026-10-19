#pragma once

#include <print>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#include <glm/glm.hpp>

// one vertex, for now just the position x y z
//    colors in obj files belong to materials (a separate .mtl file), which we skip, so the color is set in the shader instead
struct Vertex {
   glm::vec3 pos;
};

// reads an obj file into our own format, which we can then upload to the gpu
//    obj is just a text file, so we go through it line by line and look at the first word
//    we only read positions ("v") and triangles ("f"), everything else (normals, texture coords, materials) gets skipped for now
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
         // faces can look like "f 1 2 3" or "f 1/1/1 2/2/2 3/3/3", we only want the first number of each
         //    note that std::stoi stops reading at the '/', so it handles both
         // this only works for triangles, faces with 4 or more corners would need to be split up
         //       yes those files exist, but for the sake of simplicity we assume 3 vertices per face
         for (int i = 0; i < 3; i++) {
            std::string corner;
            ss >> corner;
            // obj files count from 1, yes that is right :)
            indices.push_back(std::stoi(corner) - 1);
         }
      }
   }

   std::println("loaded {}: {} vertices, {} triangles", path, vertices.size(), indices.size() / 3);
   return true;
}
