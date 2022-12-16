//
// Created by Habi Jung on 2022/12/09.
//
#define STB_IMAGE_IMPLEMENTATION

#include <iostream>
#include <string>
#include <filesystem>
#include <random>

#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "shaders/shader.h"
#include "stb_image.h"
#include "cube.h"

using namespace std;
using namespace glm;


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
    GLFWwindow *window = glfwCreateWindow(800, 600, "Learn OpenGL", NULL, NULL);
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

    /* Configure global OpenGL state */
    glEnable(GL_DEPTH_TEST);

    /* Set up mesh data */
    unsigned int indices[] = {
            0, 1, 3,  // first triangle
            1, 2, 3   // second triangle
    };

    // Set up random cube position
    vec3 cubePositions[10];
    std::random_device rd;
    std::default_random_engine gen(rd());
    std::uniform_real_distribution<float> disXY(-3, 3);
    std::uniform_real_distribution<float> disZ(-10, -3);

    for (auto &cubePosition: cubePositions) {
        cubePosition = vec3(disXY(gen), disXY(gen), disZ(gen));
    }

    /* Set up buffers */
    // Generate & Bind VBO, VAO, EBO
    unsigned int VBO, VAO, EBO;
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
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *) 0);
    glEnableVertexAttribArray(0);
    // texture attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *) (3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    /* Load & Create textures */
    unsigned int texture1, texture2;

    // texture1
    glGenTextures(1, &texture1);
    glBindTexture(GL_TEXTURE_2D, texture1);
    // texture1: Texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
    // texture1: Texture 축소 & 확대
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // texture1: Load image, create texture, and generate mipmaps with STB
    int width, height, nrChannels;
    filesystem::path imgPath = "../img/container.jpg";
    unsigned char *data = stbi_load(imgPath.string().c_str(), &width, &height, &nrChannels, 0);

    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        cout << "Failed to load texture1" << endl;
    }
    // 텍스처와 밉맵 생성 완료 후 이미지 메모리 해제
    stbi_image_free(data);

    // texture 2
    glGenTextures(1, &texture2);
    glBindTexture(GL_TEXTURE_2D, texture2);
    // texture2: Texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
    // texture2: Texture 축소 & 확대
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // texture2: Load image, create texture, and generate mipmaps with STB
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

    // TODO: Shader 함수 추가
    // 각 sampler 변수에 texture 맞춰주기
    ourShader.use();
    ourShader.setInt("texture1", 0);
    ourShader.setInt("texture2", 1);

    /* Render Loop */
    while (!glfwWindowShouldClose(window)) {
        processInput(window);

        ourShader.use();
        ourShader.setFloat("someUniform", 1.0f);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Bind textures
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);

        // Create transformations
        mat4 view = mat4(1.0f);
        mat4 projection = mat4(1.0f);
        view = translate(view, vec3(0.0f, 0.0f, -3.0f));
        projection = perspective(radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);

        // Get matrix's uniform location and Set matrix
        ourShader.use();
        int viewLoc = glGetUniformLocation(ourShader.ID, "view");
        int projectionLoc = glGetUniformLocation(ourShader.ID, "projection");
        // Pass locations to the shaders
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, value_ptr(view)); // Method 1
        glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, &projection[0][0]); // Method 2

        // Render container
        glBindVertexArray(VAO);
        // Specify indices를 사용하지 않으므로 glDrawArrays를 사용
        for (unsigned int i = 0; i < 10; i++) {
            float angle = 20.0f * (float) i;
            mat4 model = mat4(1.0f);
            model = translate(model, cubePositions[i]);
            model = rotate(model, (float) glfwGetTime() * radians(angle), vec3(1.0f, 0.3f, 0.5f));

            // Pass locations to the shaders
            ourShader.setMat4("model", model); // Method 3

            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

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
