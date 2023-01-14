#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform int texture_type;
uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;
uniform sampler2D texture_normal1;
uniform sampler2D texture_height1;


void main() {
    if (texture_type == 1) {
        FragColor = texture(texture_diffuse1, TexCoords);
    }
    else if (texture_type == 2) {
        FragColor = texture(texture_specular1, TexCoords);
    }
    else if (texture_type == 3) {
        FragColor = texture(texture_normal1, TexCoords);
    }
    else if (texture_type == 4) {
        FragColor = texture(texture_height1, TexCoords);
    }
}
