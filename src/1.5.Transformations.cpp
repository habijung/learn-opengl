//
// Created by habi on 10/17/2022.
//
#include <iostream>
#include <string>
#include <filesystem>
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "shaders/shader.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

using namespace std;


/* Prototypes */
void framebuffer_size_callback(GLFWwindow *window, int width, int height);

void processInput(GLFWwindow *window);


/* Main */
int main() {
    cout << "Run Main()" << endl;

    /* GLFW 초기화 */
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    /* Window object 생성 */
    GLFWwindow *window = glfwCreateWindow(800, 600, "Shaders", NULL, NULL);
    if (window == NULL) {
        cout << "Failed to creat GLFW window" << endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    /* Callbacks */
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    /* Initialize GLAD */
    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        cout << "Failed to initialize GLAD" << endl;
        return -1;
    }

    /* Shader Settings */
    string dirPath = "../include/shaders/";
    string vertexPath = dirPath + "shader.vs";
    string fragmentPath = dirPath + "shader.fs";
    Shader ourShader(vertexPath.c_str(), fragmentPath.c_str());

    /* Set up vertex & index data */
    float vertices[] = {
            // positions                      // colors                     // texture coords
            0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,         // top right
            0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,      // bottom right
            -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,  // bottom left
            -0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f    // top left
    };
    unsigned int indices[] = {
            0, 1, 3,   // first triangle
            1, 2, 3    // second triangle
    };
    float textCoords[] = {
            0.0f, 0.0f,     // lower-left corner
            1.0f, 0.0f,     // lower-right corner
            0.5f, 1.0f,     // top-center corner
    };

    /* GLM test */
    glm::vec4 vec(1.0f, 0.0f, 0.0f, 1.0f);
    glm::mat4 trans = glm::mat4(1.0f);
    trans = glm::translate(trans, glm::vec3(1.0f, 1.0f, 0.0f));
    vec = trans * vec;
    cout << "x: " << vec.x << ", y: " << vec.y << ", z: " << vec.z << endl;

    /* Set up buffers */
    unsigned int VBO, VAO, EBO;

    // Generate & Bind VBO, VAO, EBO
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    // Copy vertices array in a buffer for OpenGL to use
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // Linking vertex attributes pointers
    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) 0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) (3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // texture attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) (6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    // glVertexAttribPointer()를 통해서 vertex attribute의 vertex buffer object를 가져왔기 때문에, unbind가 가능함
    // 그러나 VAO가 활성화 되어 있는 동안 EBO는 unbind 하면 안됨
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    /* texture1 settings */
    unsigned int texture1, texture2;
    glGenTextures(1, &texture1);
    glBindTexture(GL_TEXTURE_2D, texture1);

    /* Textures option settings */
    // Wrapping 반복에 관한 설정
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
    // 축소 & 확대에 관한 설정
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    /* STB settings (texture1) */
    filesystem::path imgPath = "../img/container.jpg";
    int width, height, nrChannels;
    unsigned char *data = stbi_load(imgPath.string().c_str(), &width, &height, &nrChannels, 0);

    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        cout << "Failed to load texture1" << endl;
    }
    stbi_image_free(data); // 텍스처와 밉맵 생성 완료 후 이미지 메모리 해제

    /* texture2 settings */
    glGenTextures(1, &texture2);
    glBindTexture(GL_TEXTURE_2D, texture2);
    // Wrapping 반복에 관한 설정
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
    // 축소 & 확대에 관한 설정
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    /* STB settings (texture2) */
    stbi_set_flip_vertically_on_load(true);
    imgPath = "../img/awesomeface.png";
    data = stbi_load(imgPath.string().c_str(), &width, &height, &nrChannels, 0);

    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        cout << "Failed to load texture2" << endl;
    }
    stbi_image_free(data);

    // 각 sampler 변수에 texture 맞춰주기
    ourShader.use();
    glUniform1i(glGetUniformLocation(ourShader.ID, "texture1"), 0);
    ourShader.setInt("texture2", 1);

    /* Render Loop */
    while (!glfwWindowShouldClose(window)) {
        processInput(window);

        ourShader.use();
        ourShader.setFloat("someUniform", 1.0f);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);

        ourShader.use();
        glBindVertexArray(VAO);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Optional: De-allocate all resources once they've outlived their purpose
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

    glfwTerminate();

    return 0;
}


/* Functions */
void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}
