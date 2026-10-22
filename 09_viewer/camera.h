#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <algorithm>
#include <cmath>

// orbit camera -- rotates around a center point at a fixed distance
class Camera {
 public:
   Camera(glm::vec3 center = glm::vec3(0.0f)) {
      this->center = center;
      update();
   }

   glm::mat4 getViewMatrix() {
      glm::vec3 up = orientation * world_up;
      return glm::lookAt(position, center, up);
   }
   glm::mat4 getProjMatrix(int width, int height) {
      return glm::perspective(glm::radians(fov), (float)width / (float)height, near_plane, far_plane);
   }

   // fit the camera so the given sphere is fully in view
   void frame(glm::vec3 center, float radius) {
      radius = std::max(radius, 0.01f);
      this->center = center;
      distance = radius / tan(glm::radians(fov * 0.5f)) * 1.2f;
      far_plane = std::max(far_plane, radius * 4.0f);
      update();
   }

   void orbit(glm::vec2 prev, glm::vec2 curr, int width, int height) {
      glm::vec3 p1 = projectToSphere(prev.x, prev.y, width, height);
      glm::vec3 p2 = projectToSphere(curr.x, curr.y, width, height);

      // cross product gives the rotation axis, dot product gives the angle
      glm::vec3 axis = glm::cross(p1, p2);
      float axis_len = glm::length(axis);
      if (axis_len < 1e-6f)
         return;

      float angle = std::acos(glm::clamp(glm::dot(p1, p2), -1.0f, 1.0f)) * sensitivity;

      glm::vec3 world_axis = orientation * (axis / axis_len);
      orientation = glm::normalize(glm::angleAxis(-angle, world_axis) * orientation);
      update();
   }
   void pan(float dx, float dy) {
      glm::vec3 right = orientation * glm::vec3(1.0f, 0.0f, 0.0f);
      glm::vec3 up = orientation * world_up;
      center += (-right * dx + up * dy) * pan_speed * distance;
      update();
   }
   void zoom(float delta) {
      distance = std::max(distance - delta * zoom_speed * distance, 0.01f);
      update();
   }

   glm::vec3 position{};
   glm::quat orientation{1.0f, 0.0f, 0.0f, 0.0f};  // quaternion: avoids gimbal lock
   glm::vec3 center{0.0f};
   float distance = 10.0f;

   float sensitivity = 1.4f;
   float pan_speed = 0.0003f;
   float zoom_speed = 0.05f;
   float fov = 20.0f;
   float near_plane = 0.1f;
   float far_plane = 10000.0f;

 private:
   glm::vec3 world_up{0.0f, 1.0f, 0.0f};

   void update() {
      position = center + orientation * glm::vec3(0.0f, 0.0f, distance);
   }

   // maps a screen pixel to a point on an imaginary unit sphere
   // dragging two such points gives a natural arcball rotation
   static glm::vec3 projectToSphere(float x, float y, int width, int height) {
      float nx = (2.0f * x - (float)width) / (float)width;
      float ny = ((float)height - 2.0f * y) / (float)height;  // screen y goes down
      float len2 = nx * nx + ny * ny;
      float nz = len2 <= 0.5f ? std::sqrt(1.0f - len2) : 0.5f / std::sqrt(len2);
      return glm::normalize(glm::vec3(nx, ny, nz));
   }
};
