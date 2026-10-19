#version 330 core
layout(location = 0) in vec3 vpos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

// out vec3 acolor;

void main() {
   gl_Position = projection * view * model * vec4(vpos, 1.0);

   // since we dont have a color vertex attribute, we can define it here from positions
   // acolor = vpos + 0.5;
}
