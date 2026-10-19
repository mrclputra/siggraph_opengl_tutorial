#pragma once

#include <print>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>

#include <glm/glm.hpp>

struct Vertex {
   glm::vec3 pos;
   glm::vec3 normal;
};

// reads an obj file, a more complete version of 08_lighting
//    positions "v x y z"
//    normals "vn x y z"
//    faces "f" with any number of corners, each corner can be "1", "1/2", "1//3" or "1/2/3"
//    texture coordinates and materials (colors) get skipped
bool loadObj(const std::string &path, std::vector<Vertex> &vertices, std::vector<unsigned int> &indices) {
   std::ifstream file(path);
   if (!file) {
      std::println("error, could not open file {}", path);
      return false;
   }

   std::vector<glm::vec3> positions;
   std::vector<glm::vec3> normals;

   // in an obj a corner can pair any position with any normal, but opengl wants one index per vertex
   //    so every new (position, normal) pair becomes its own vertex, and pairs we have seen before get reused
   //    e.g. a cube corner touches 3 faces with 3 different normals, so it becomes 3 vertices
   std::map<std::pair<int, int>, unsigned int> seen;

   std::string line;
   while (std::getline(file, line)) {
      std::stringstream ss(line);
      std::string type;
      ss >> type;

      if (type == "v") {
         glm::vec3 pos;
         ss >> pos.x >> pos.y >> pos.z;
         positions.push_back(pos);
      } else if (type == "vn") {
         glm::vec3 normal;
         ss >> normal.x >> normal.y >> normal.z;
         normals.push_back(normal);
      } else if (type == "f") {
         // read all the corners of this face
         std::vector<unsigned int> face;
         std::string corner;
         while (ss >> corner) {
            int p = std::stoi(corner) - 1;  // obj counts from 1
            int n = -1;                     // -1 means no normal

            // the normal index is the number after the last slash, if there are 2 slashes
            size_t slash = corner.rfind('/');
            if (slash != std::string::npos && corner.find('/') != slash)
               n = std::stoi(corner.substr(slash + 1)) - 1;

            std::pair<int, int> key(p, n);
            if (!seen.contains(key)) {
               Vertex vertex;
               vertex.pos = positions[p];
               vertex.normal = glm::vec3(0.0f);
               if (n >= 0)
                  vertex.normal = normals[n];

               seen[key] = vertices.size();
               vertices.push_back(vertex);
            }
            face.push_back(seen[key]);
         }

         // faces with more than 3 corners get split into triangles that all share the first corner
         //    a quad 0 1 2 3 becomes 0 1 2 and 0 2 3
         for (size_t i = 1; i + 1 < face.size(); i++) {
            indices.push_back(face[0]);
            indices.push_back(face[i]);
            indices.push_back(face[i + 1]);
         }
      }
   }

   // no normals in the file, so we make our own
   //    every triangle adds its direction to its 3 corners, and after normalizing we get smooth shading
   //    glm::cross() of two edges gives a vector pointing straight out of the triangle
   if (normals.empty()) {
      for (size_t i = 0; i < indices.size(); i += 3) {
         Vertex &a = vertices[indices[i]];
         Vertex &b = vertices[indices[i + 1]];
         Vertex &c = vertices[indices[i + 2]];
         glm::vec3 normal = glm::cross(b.pos - a.pos, c.pos - a.pos);
         a.normal += normal;
         b.normal += normal;
         c.normal += normal;
      }
      for (Vertex &vertex : vertices) {
         if (glm::length(vertex.normal) > 0.0f)
            vertex.normal = glm::normalize(vertex.normal);
      }
   }

   std::println("loaded {}: {} vertices, {} triangles", path, vertices.size(), indices.size() / 3);
   return true;
}
