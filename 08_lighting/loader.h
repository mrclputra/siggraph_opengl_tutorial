#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#include <glm/glm.hpp>

// color is replaced by the normal, the direction the surface is facing at this vertex
struct Vertex {
   glm::vec3 pos;
   glm::vec3 normal;
};

// same as 07_loader, but now we also read the normals ("vn")
bool loadObj(const std::string &path, std::vector<Vertex> &vertices, std::vector<unsigned int> &indices) {
   std::ifstream file(path);
   if (!file) {
      std::cout << "error, could not open file " << path << "\n";
      return false;
   }

   // normals are listed separately in the file, the faces say which normal goes with which vertex
   std::vector<glm::vec3> normals;

   std::string line;
   while (std::getline(file, line)) {
      std::stringstream ss(line);
      std::string type;
      ss >> type;

      if (type == "v") {
         Vertex vertex;
         ss >> vertex.pos.x >> vertex.pos.y >> vertex.pos.z;
         vertex.normal = glm::vec3(0.0f);
         vertices.push_back(vertex);
      } else if (type == "vn") {
         glm::vec3 normal;
         ss >> normal.x >> normal.y >> normal.z;
         normals.push_back(normal);
      } else if (type == "f") {
         for (int i = 0; i < 3; i++) {
            std::string corner;
            ss >> corner;
            unsigned int index = std::stoi(corner) - 1;
            indices.push_back(index);

            // corners look like "1//1" or "1/1/1", the normal index is the number after the last slash
            //    this assumes every vertex only has one normal, which is true for smooth meshes like the dragon
            //    something with sharp edges like a cube would need its corners duplicated, one per normal
            size_t slash = corner.rfind('/');
            if (slash != std::string::npos && corner.find('/') != slash) {
               vertices[index].normal = normals[std::stoi(corner.substr(slash + 1)) - 1];
            }
         }
      }
   }

   std::cout << "loaded " << path << ": " << vertices.size() << " vertices, " << indices.size() / 3 << " triangles\n";
   return true;
}
