#pragma once

#include <GLFW/glfw3.h>

// keeps track of the keyboard and mouse, taken from caldera
//    the callbacks from 02_input only save what happened, and the main loop reads it whenever it wants
class Input {
 public:
   Input(GLFWwindow *window) {
      // glfw callbacks can't be normal member functions, so we give glfw a pointer to this object
      //    and get it back inside the callbacks with glfwGetWindowUserPointer
      glfwSetWindowUserPointer(window, this);
      glfwSetKeyCallback(window, keyCallback);
      glfwSetCursorPosCallback(window, cursorPosCallback);
      glfwSetScrollCallback(window, scrollCallback);
      glfwSetMouseButtonCallback(window, mouseButtonCallback);

      glfwGetCursorPos(window, &cursor_x, &cursor_y);
   }

   // call once every frame, after reading the input
   //    the movement adds up between frames, so it has to be reset
   void update() {
      cursor_dx = 0.0f;
      cursor_dy = 0.0f;
      scroll_delta = 0.0f;
   }

   bool isKeyDown(int key) const {
      return keys_current[key];
   }
   bool isMouseDown(int key) const {
      return buttons_current[key];
   }

   double cursor_x = 0.0f;
   double cursor_y = 0.0f;
   double cursor_dx = 0.0f;
   double cursor_dy = 0.0f;
   double scroll_delta = 0.0f;

 private:
   static void keyCallback(GLFWwindow *window, int key, int, int action, int) {
      if (key < 0 || key >= num_keys)
         return;  // GLFW_KEY_UNKNOWN is -1

      Input *input = static_cast<Input *>(glfwGetWindowUserPointer(window));
      if (action == GLFW_PRESS)
         input->keys_current[key] = true;
      else if (action == GLFW_RELEASE)
         input->keys_current[key] = false;
   }
   static void cursorPosCallback(GLFWwindow *window, double x, double y) {
      Input *input = static_cast<Input *>(glfwGetWindowUserPointer(window));

      if (input->first_cursor_event) {
         input->cursor_x = x;
         input->cursor_y = y;
         input->first_cursor_event = false;
      }

      input->cursor_dx += x - input->cursor_x;
      input->cursor_dy += y - input->cursor_y;
      input->cursor_x = x;
      input->cursor_y = y;
   }
   static void scrollCallback(GLFWwindow *window, double, double y_offset) {
      Input *input = static_cast<Input *>(glfwGetWindowUserPointer(window));
      input->scroll_delta += y_offset;
   }
   static void mouseButtonCallback(GLFWwindow *window, int button, int action, int) {
      if (button < 0 || button >= num_buttons)
         return;

      Input *input = static_cast<Input *>(glfwGetWindowUserPointer(window));
      if (action == GLFW_PRESS)
         input->buttons_current[button] = true;
      else if (action == GLFW_RELEASE)
         input->buttons_current[button] = false;
   }

   static constexpr int num_keys = GLFW_KEY_LAST + 1;
   bool keys_current[num_keys]{};

   static constexpr int num_buttons = GLFW_MOUSE_BUTTON_LAST + 1;
   bool buttons_current[num_buttons]{};

   bool first_cursor_event = true;
};
