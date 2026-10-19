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

   // the lighting is done in world space, so the fragment shader needs the position and normal after the model matrix
   apos = vec3(model * vec4(vpos, 1.0));

   // a normal is a direction, so it only gets rotated, not moved
   //    mat3(model) drops the moving part of the matrix, this only works since we don't stretch the model
   anormal = mat3(model) * vnormal;

   acolor = vcolor;
}
