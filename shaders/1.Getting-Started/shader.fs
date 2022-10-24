#version 330 core
out vec4 FragColor;
in vec3 ourColor; // vertex shader의 결과를 받아옴

void main() {
    FragColor = vec4(ourColor, 1.0);
}
