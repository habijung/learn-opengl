#version 330 core

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};

struct Light {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;
out vec4 FragColor;

uniform vec3 lightPos;
uniform vec3 viewPos;
uniform Material material;
uniform Light light;

void main() {
    // Ambient
    // 조명으로 색상을 처리하기 때문에 ambient 색상을 diffuse 색상과 같은 값으로 처리함
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));

    // Diffuse
    // 조명의 방향 계산
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);
    // 조명의 각도 계산
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));

    // Specular Light
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));

    vec3 result = ambient + diffuse + specular;
    FragColor = vec4(result, 1.0);
}
