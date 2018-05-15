#version 330 core

in vec2 UV;
in vec4 xx;
out vec3 color;

uniform sampler2D myTexture;

void main(){
	color = (xx.x + 1)  * (xx.y + 1) * 0.3 * (xx.x + 1) * texture( myTexture, UV ).rgb;
}