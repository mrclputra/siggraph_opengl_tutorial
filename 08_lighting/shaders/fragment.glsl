#version 330 core
in vec3 apos;
in vec3 anormal;
out vec4 color;

uniform vec3 light_pos;
uniform vec3 view_pos;

void main() {
   vec3 object_color = vec3(1.0, 1.0, 1.0); // color of the object, white
   vec3 light_color = vec3(1.0, 0.95, 0.85); // color of the light

   // the normal gets blended between the vertices,
   //    so it's not exactly length 1 anymore
   vec3 normal = normalize(anormal);
   vec3 to_light = normalize(light_pos - apos);
   vec3 to_view = normalize(view_pos - apos);

   // ambient light
   vec3 ambient = 0.1 * light_color;

   // diffuse
   //    the more a surface faces the light, the brighter it is
   //    the dot product of 2 vectors (unit) is 1 when they point the same way, and 0 at >90
   float diff = max(dot(normal, to_light), 0.0);
   vec3 diffuse = diff * light_color;

   // specular
   //    this is the shiny highlight
   //    reflect() mirrors the light around the normal
   vec3 reflected = reflect(-to_light, normal);
   float spec = pow(max(dot(to_view, reflected), 0.0), 32.0);
   vec3 specular = 0.5 * spec * light_color;

   // put it all together now
   color = vec4((ambient + diffuse) * object_color + specular, 1.0);
}
