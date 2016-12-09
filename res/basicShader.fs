#version 150

in vec2 texCoord0;
in vec3 normal0;

out vec4 outColor;

uniform sampler2D sampler;
uniform vec3 lightDirection;

void main()
{
	vec2 flipped_texcoord = vec2(texCoord0.x, 1.0 - texCoord0.y);
	outColor =
		texture(sampler, flipped_texcoord) *
	//	vec4(0.21, 0.086, 0.33, 1.0) * 1.5 *
		clamp(dot(-lightDirection * 1.5, normal0), 0.0, 1.0);

}
