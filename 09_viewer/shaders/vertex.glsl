#version 330 core
layout(location = 0) in vec3 vpos;
layout(location = 1) in vec3 vcolor;
layout(location = 2) in vec3 vnormal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 apos;
out vec3 acolor;
out vec3 anormal;

void main() {
   gl_Position = projection * view * model * vec4(vpos, 1.0);
   apos = vec3(model * vec4(vpos, 1.0));
   anormal = mat3(model) * vnormal;
   acolor = vcolor;
}
