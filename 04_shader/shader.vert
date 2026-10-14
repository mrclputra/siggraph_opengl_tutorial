#version 330 core  // glsl version, 330 matches opengl 3.3

// input from the vbo, location 0 is what we set in glVertexAttribPointer
layout(location = 0) in vec3 pos;

// runs once for every vertex
void main() {
   gl_Position = vec4(pos, 1.0);  // built-in output, the final position of this vertex
}
