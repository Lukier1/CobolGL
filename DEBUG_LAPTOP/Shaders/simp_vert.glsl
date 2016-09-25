#version 400
layout(location = 0) in vec3 vp;
layout(location = 1) in vec3 vc;

uniform mat4 matrix;

out vec3 colour;



void main () {
  colour = vc;
  gl_Position = matrix*vec4 (vp, 1.0);
};