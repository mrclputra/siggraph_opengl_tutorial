#version 330 core  // glsl version, 330 matches opengl 3.3

// inputs from the vbo, the locations are what we set in glVertexAttribPointer
layout(location = 0) in vec3 vpos;
layout(location = 1) in vec3 vcolor;

out vec3 acolor;

// runs once for every vertex
void main() {
   gl_Position = vec4(vpos, 1.0);  // built-in output, the final position of this vertex
   acolor = vcolor;
}
