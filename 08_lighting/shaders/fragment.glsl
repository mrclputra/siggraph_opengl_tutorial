#version 330 core
in vec3 apos;
in vec3 anormal;
out vec4 color;

uniform vec3 light_pos;
uniform vec3 view_pos;

void main() {
   vec3 object_color = vec3(0.85, 0.8, 0.7);  // warm ivory, a light color shows the shading best
   vec3 light_color = vec3(1.0, 0.95, 0.85);  // slightly warm light

   // the normal gets blended between the vertices, so it's not exactly length 1 anymore
   vec3 normal = normalize(anormal);
   vec3 to_light = normalize(light_pos - apos);
   vec3 to_view = normalize(view_pos - apos);

   // ambient, a little bit of light everywhere so the dark side isn't pure black
   vec3 ambient = 0.1 * light_color;

   // diffuse, the more the surface faces the light the brighter it is
   //    dot() of two length 1 vectors is 1 when they point the same way, 0 at 90 degrees and negative when facing away
   float diff = max(dot(normal, to_light), 0.0);
   vec3 diffuse = diff * light_color;

   // specular, the shiny highlight, strongest where the light bounces straight into the camera
   //    reflect() mirrors the light around the normal, the 32 is how small and sharp the highlight is
   vec3 reflected = reflect(-to_light, normal);
   float spec = pow(max(dot(to_view, reflected), 0.0), 32.0);
   vec3 specular = 0.5 * spec * light_color;

   color = vec4((ambient + diffuse) * object_color + specular, 1.0);
}
