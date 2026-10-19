#version 330 core
in vec3 apos;
in vec3 anormal;
in vec3 acolor;
out vec4 color;

uniform vec3 light_pos;
uniform vec3 view_pos;

void main() {
   vec3 light_color = vec3(1.0, 0.95, 0.85);

   vec3 normal = normalize(anormal);
   vec3 to_light = normalize(light_pos - apos);
   vec3 to_view = normalize(view_pos - apos);

   vec3 ambient = 0.1 * light_color;

   float diff = max(dot(normal, to_light), 0.0);
   vec3 diffuse = diff * light_color;

   vec3 reflected = reflect(-to_light, normal);
   float spec = pow(max(dot(to_view, reflected), 0.0), 32.0);
   vec3 specular = 0.5 * spec * light_color;

   // the color comes from the model
   color = vec4((ambient + diffuse) * acolor + specular, 1.0);
}
