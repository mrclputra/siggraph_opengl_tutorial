#version 330 core

in vec3 acolor;

// output, the color of this pixel
out vec4 color;

// uniforms are set from c++, and stay the same for every vertex and pixel in a draw call
uniform float time;

// runs once for every pixel the triangle covers
void main() {
   float brightness = sin(time) * 0.5 + 0.5;  // sin goes from -1 to 1, this maps it to 0 to 1
   color = vec4(acolor * brightness, 1.0);    // r, g, b, a
}
