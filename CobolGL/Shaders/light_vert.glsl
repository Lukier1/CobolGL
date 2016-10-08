#version 400

layout (location = 0) in vec3 vertex_position;
layout (location = 1) in vec3 vertex_normal;
layout (location = 2) in vec2 vt;
uniform mat4 mv_mat, mvp_mat;

out vec3 position_eye, normal_eye;
out vec2 tex_coord;
void main() {
	tex_coord = vt;
	position_eye = vec3(mv_mat*vec4(vertex_position,1.0));
	normal_eye = normalize(mat3(mv_mat)*vec3(vertex_normal));

	gl_Position = mvp_mat * vec4(vertex_position, 1.0);
}