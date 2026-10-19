#version 330 core
layout(location = 0) in vec3 vpos;
layout(location = 1) in vec3 vcolor;

// set from c++ every frame, see main.cpp for what each one does
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 acolor;

void main() {
   // read it right to left: model first, then view, then projection
   //    the 1.0 at the end (w) is needed so the matrices can also move the point, not just rotate it
   gl_Position = projection * view * model * vec4(vpos, 1.0);
   acolor = vcolor;
}
