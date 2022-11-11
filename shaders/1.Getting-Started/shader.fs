#version 330 core
out vec4 FragColor;
in vec3 ourColor; // vertex shader의 결과를 받아옴
in vec2 TexCoord;

uniform sampler2D ourTexture;

void main() {
    FragColor = texture(ourTexture, TexCoord);
}
