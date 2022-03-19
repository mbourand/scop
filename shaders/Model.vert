#version 450 core
precision mediump float;

in vec3 a_position;
in vec3 a_normal;
in vec3 a_color;

uniform mat4 u_mvp;
uniform mat4 u_rotation;
uniform mat4 u_translation;

out vec3 v_position;
out vec3 v_normal;
out vec3 v_color;
out vec2 v_texCoord;

void main()
{
	v_position = a_position;
	v_normal = a_normal;
	v_color = a_color;
	gl_Position = u_mvp * u_translation * u_rotation * vec4(a_position, 1.0);
	v_texCoord = vec2(v_position.z, v_position.y);
}
