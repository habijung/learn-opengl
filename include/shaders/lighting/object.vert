#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
    FragPos = vec3(model * vec4(aPos, 1.0));

    // 비균일 Scale이 적용되면 normal도 변경하기.
    // 그러나, inverse는 비용이 많이 들기 때문에 복잡한 작업에서는 사용하지 않기.
    Normal = mat3(transpose(inverse(model))) * aNormal;

    TexCoords = aTexCoords;

    gl_Position = projection * view * vec4(FragPos, 1.0);
}
