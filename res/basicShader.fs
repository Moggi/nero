#version 150

in vec2 texCoord0;
in vec3 normal0;

out vec4 outColor;

uniform sampler2D sampler;
uniform vec3 lightDirection;
uniform vec3 lightDirection2;

void main()
{
	vec2 flipped_texcoord = vec2(texCoord0.x, 1.0 - texCoord0.y);
	outColor =
		texture(sampler, flipped_texcoord) *
	//	vec4(0.21, 0.086, 0.33, 1.0) * 1.5 *
	//	(
	//		gl_LightSource[0].ambient +
			(
	//	0.0f, 0.0f, 1.0f		BACK
				clamp(dot(-lightDirection * 0.9, normal0), 0.0, 0.9) +

	//	0.0f, -1.0f, 0.0f		TOP
				clamp(dot(-lightDirection2* 0.9, normal0), 0.0, 0.9) +

	//							FRONT above
	//			clamp(dot(-vec3(0.0,0.0,-1.0)* 0.9, normal0), 0.0, 0.9) +

	//							FRONT below
				clamp(dot(-vec3(0.0,1.0,-1.0)* 0.9, normal0), 0.0, 0.9) +

	//							LEFT
				clamp(dot(-vec3(1.0,0.0,0.0)* 0.9, normal0), 0.0, 0.9) +

	//							RIGHT
				clamp(dot(-vec3(-1.0,0.0,0.0)* 0.9, normal0), 0.0, 0.9)
			);
	//	);

}
