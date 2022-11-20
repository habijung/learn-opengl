#version 330 core
out vec4 FragColor;
in vec3 ourColor; // vertex shader의 결과를 받아옴
in vec2 TexCoord;

uniform sampler2D ourTexture;
uniform sampler2D texture1;
uniform sampler2D texture2;

void main() {
//     FragColor = texture(ourTexture, TexCoord) * vec4(ourColor, 1.0);
    FragColor = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), 0.2);
}
