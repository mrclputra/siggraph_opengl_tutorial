#version 330 core
in vec3 acolor;
out vec4 color;

void main() {
   color = vec4(acolor, 1.0);
}
