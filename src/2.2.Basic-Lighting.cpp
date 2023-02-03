//
// Created by habi on 1/27/2023.
//

#define GLFW_INCLUDE_NONE
#define STB_IMAGE_IMPLEMENTATION

#include <filesystem>
#include <iostream>
#include <random>
#include <string>

#include "GLFW/glfw3.h"
#include "glad/glad.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "camera.h"
#include "cube.h"
#include "shaders/shader.h"
#include "stb_image.h"

using namespace std;
using namespace glm;


/* Prototypes */
void framebuffer_size_callback(GLFWwindow *window, int width, int height);

void processInput(GLFWwindow *window);

void mouse_callback(GLFWwindow *window, double xpos, double ypos);

void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);

/* Screen */
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

/* Timing */
float deltaTime = 0.0f;// Time between current frame and last frame
float lastFrame = 0.0f;// Time of last frame

/* Camera */
Camera camera(vec3(0.0f, 0.0f, 5.0f));
bool firstMouse = true;
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;

/* Lighting */
vec3 lightPos(1.2f, 1.0f, 2.0f);


/* Main */
int main() {
    cout << "Run \"Learn OpenGL\"" << endl;

    /* GLFW 초기화 */
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    /* Window object 생성 */
    GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Learn OpenGL", NULL, NULL);
    if (window == NULL) {
        cout << "Failed to creat GLFW window" << endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    /* Callbacks */
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    /* Mouse Input Settings */
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    /* Initialize GLAD */
    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        cout << "Failed to initialize GLAD" << endl;
        return -1;
    }

    /* Configure global OpenGL state */
    glEnable(GL_DEPTH_TEST);

    /* Shader Settings */
    string vs = "include/shaders/lighting/object.vert";
    string fs = "include/shaders/lighting/object.frag";
    Shader objectShader(vs.c_str(), fs.c_str());

    vs = "include/shaders/lighting/lighting.vert";
    fs = "include/shaders/lighting/lighting.frag";
    Shader lightingShader(vs.c_str(), fs.c_str());

    /* Set Up buffers */
    // Generate object VAO
    unsigned int VBO, objectVAO;
    glGenBuffers(1, &VBO);
    glGenVertexArrays(1, &objectVAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cube_normal), cube_normal, GL_STATIC_DRAW);

    glBindVertexArray(objectVAO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *) 0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *) (3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Generate lighting VAO
    unsigned int lightingVAO;
    glGenVertexArrays(1, &lightingVAO);

    // Only need to bind the VBO, the container's VBO's data already contains the data.
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBindVertexArray(lightingVAO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *) 0);
    glEnableVertexAttribArray(0);

    /* Set up random cube position */
    vec3 cubePositions[10];
    std::random_device rd;
    std::default_random_engine gen(rd());
    std::uniform_real_distribution<float> disXY(-3, 3);
    std::uniform_real_distribution<float> disZ(-5, 0);

    for (auto &cubePosition: cubePositions) {
        cubePosition = vec3(disXY(gen), disXY(gen), disZ(gen));
    }

    /* ------------------------ */
    /* -     Render Loop      - */
    /* ------------------------ */
    while (!glfwWindowShouldClose(window)) {
        // Per-Frame time logic
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(window);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        objectShader.use();
        objectShader.setVec3("objectColor", vec3(1.0f, 0.5f, 0.31f));
        objectShader.setVec3("lightColor", vec3(1.0f, 1.0f, 1.0f));
        objectShader.setVec3("lightPos", lightPos);

        // Transformations
        mat4 projection = perspective(radians(camera.Zoom), (float) SCR_WIDTH / (float) SCR_HEIGHT, 0.1f, 100.0f);
        mat4 view = camera.GetViewMatrix();
        objectShader.setMat4("projection", projection);
        objectShader.setMat4("view", view);
        objectShader.setVec3("viewPos", camera.Position);

        // Render random cube objects
        //        for (unsigned int i = 0; i < 10; i++) {
        //            float angle = 20.0f * (float) i;
        //            mat4 model = mat4(1.0f);
        //            model = translate(model, cubePositions[i]);
        //            model = rotate(model, (float) glfwGetTime() * radians(angle), vec3(1.0f, 0.3f, 0.5f));
        //            objectShader.setMat4("model", model);
        //
        //            glBindVertexArray(objectVAO);
        //            glDrawArrays(GL_TRIANGLES, 0, 36);
        //        }

        // Render cube objects
        mat4 modelObject = mat4(1.0f);
        objectShader.setMat4("model", modelObject);

        glBindVertexArray(objectVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // Render lighting
        lightingShader.use();
        lightingShader.setMat4("projection", projection);
        lightingShader.setMat4("view", view);

        mat4 model = mat4(1.0f);
        model = translate(model, lightPos);
        model = scale(model, glm::vec3(0.5f));
        lightingShader.setMat4("model", model);

        glBindVertexArray(lightingVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Optional: De-allocate all resources once they've outlived their purpose
    glDeleteVertexArrays(1, &objectVAO);
    glDeleteVertexArrays(1, &lightingVAO);
    glDeleteBuffers(1, &VBO);

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

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        camera.ProcessKeyboard(FORWARD, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        camera.ProcessKeyboard(LEFT, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        camera.ProcessKeyboard(RIGHT, deltaTime);
    }
}

void mouse_callback(GLFWwindow *window, double xposIn, double yposIn) {
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;// reversed since y-coordinates range from bottom to top
    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow *window, double xoffset, double yoffset) {
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}
