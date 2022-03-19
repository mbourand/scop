#version 450 core
precision mediump float;

struct Material
{
	vec3 color;
	float ambient;
	float diffuse;
	float specular;
	float shininess;
};

struct Light
{
	vec3 direction;
	vec3 color;
	float intensity;
};

in vec3 v_position;
in vec3 v_normal;
in vec3 v_color;
in vec2 v_texCoord;

uniform Material u_material;
uniform Light u_light;
uniform vec3 u_cameraPos;
uniform sampler2D u_texture;
uniform float u_transitionState;

out vec4 color;

void main()
{
	vec4 mixedColor = mix(texture(u_texture, v_texCoord), vec4(v_color, 1.0), u_transitionState);

	if (length(v_normal) > 0)
	{
		vec3 ambient = mixedColor.xyz * u_material.ambient * u_light.color * u_light.intensity;
		vec3 diffuse = mixedColor.xyz * u_material.diffuse * max(0, dot(-u_light.direction, v_normal)) * u_light.color * u_light.intensity;

		vec3 reflection = reflect(u_light.direction, v_normal);
		vec3 cameraDirection = normalize(u_cameraPos - v_position);
		vec3 specular = u_material.specular * pow(max(0, dot(reflection, cameraDirection)), u_material.shininess) * u_light.color * u_light.intensity;
		color = vec4(ambient + diffuse + specular, 1.0);
	}
	else
		color = mixedColor;
}
