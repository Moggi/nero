#version 150

in vec2 texCoord0;
in vec3 normal0;

out vec4 outColor;

uniform sampler2D sampler;
uniform vec3 lightDirection;
uniform vec3 lightDirection2;

void main()
{
	vec2 flipped_texcoord = vec2(1.0 - texCoord0.x, 1.0 - texCoord0.y);
	outColor = texture(sampler, flipped_texcoord);
    //outColor = texture(sampler, texCoord0);
}
