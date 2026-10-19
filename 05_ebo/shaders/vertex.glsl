#version 330 core
layout(location = 0) in vec3 vpos;
layout(location = 1) in vec3 vcolor;

out vec3 acolor;

void main() {
   gl_Position = vec4(vpos, 1.0);
   acolor = vcolor;
}
