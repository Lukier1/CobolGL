#version 400


uniform sampler2D basic_texture;

uniform vec3 LPos_world, Ls, Ld, La; // light position, specular, diffuse, ambient
uniform vec3 Ks, Kd, Ka; // color specular, diffuse, ambient
uniform float spec_power;  // specular factor

uniform mat4 view_mat;

in vec3 position_eye, normal_eye;
in vec2 tex_coord;
out vec4 frag_colour;


void main () {
   //ambient intesity
  vec3 Ia = La * Ka;

  //Diffuse intesity
  vec3 LPos_eye = vec3( view_mat * vec4(LPos_world,1.0));
  vec3 distance_to_light_eye = LPos_eye - position_eye;
  vec3 direction_to_light_eye = normalize(distance_to_light_eye);

  float dot_prod = dot(direction_to_light_eye, normal_eye);
  dot_prod = max(dot_prod, 0.0);

  vec3 Id = Ld*Kd*dot_prod;

  //Specular intestiy 
  //vec3 reflection_eye = reflect( -direction_to_light_eye, normal_eye);

  vec3 surface_to_viewer_eye = normalize(-position_eye);
  vec3 half_way_eye = normalize (surface_to_viewer_eye + direction_to_light_eye);
  float dot_prod_specular = dot(half_way_eye, normal_eye);
  dot_prod_specular = max(dot_prod_specular,0.0);
  float specular_factor = pow(dot_prod_specular, spec_power);
  vec3 Is = Ls*Ks*specular_factor;
  

  // Texture
  vec4 texel = texture2D(basic_texture, tex_coord);
  frag_colour = vec4 (vec3(texel)*(Ia+Id)+Is, 1.0);
 //	 frag_colour = vec4(normal_eye, 1.0);
};