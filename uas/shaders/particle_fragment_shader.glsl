#version 330 core

// Interpolated values from the vertex shaders
in vec2 UV;
in vec4 particlecolor;

// Ouput data
out vec4 color;

uniform sampler2D smokeTexture;
uniform sampler2D rainTexture;

void main(){
	if (particlecolor.r == 0) {
	    color = texture( smokeTexture, UV );
	} else {
	    color = texture( rainTexture, UV );
	}
}