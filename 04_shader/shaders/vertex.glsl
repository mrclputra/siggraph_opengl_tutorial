#version 330 core  // glsl version, 330 matches opengl 3.3

// inputs from the vbo, the locations are what we set in glVertexAttribPointer
layout(location = 0) in vec3 vpos;
layout(location = 1) in vec3 vcolor;

uniform float time;

out vec3 acolor;

// runs once for every vertex
void main() {
   // gl_Position is a built-in output, there are many like this
   //    but this is required
   gl_Position = vec4(vpos + vec3(sin(time), 0.0, 0.0), 1.0);
   acolor = vcolor; // pass the color on to the fragment shader
}
