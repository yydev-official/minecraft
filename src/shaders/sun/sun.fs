#version 330 core
in vec2 TexCoords;
out vec4 FragColor;
uniform sampler2D sunTexture;

void main() {
   vec4 texColor = texture(sunTexture, TexCoords);
   if(texColor.a < 0.1) discard;
   FragColor = texColor;
}