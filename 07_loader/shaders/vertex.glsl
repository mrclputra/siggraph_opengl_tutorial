#version 330 core
layout(location = 0) in vec3 vpos;
layout(location = 1) in vec3 vcolor;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 acolor;

void main() {
   gl_Position = projection * view * model * vec4(vpos, 1.0);
   acolor = vcolor;
}
