#version 150

in vec2 texCoord0;
in vec3 normal0;

out vec4 outColor;

uniform sampler2D sampler;
uniform vec3 lightDirection;

void main()
{
	//outColor = texture(sampler, texCoord0) *
	// 	vec4(0.21, 0.086, 0.33, 1.0) * 1.8 *
	//	clamp(dot(-lightDirection, normal0), 0.0, 1.0);
	
	outColor = vec4(0.21, 0.086, 0.33, 1.0) * 1.8 *
		clamp(dot(-lightDirection, normal0), 0.0, 1.0);
}
