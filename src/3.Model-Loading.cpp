//
// Created by Habi Jung on 2023/01/13.
//

#define STB_IMAGE_IMPLEMENTATION

#include <iostream>
#include <string>

#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "shaders/shader.h"
#include "camera.h"
#include "model.h"

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
float deltaTime = 0.0f; // Time between current frame and last frame
float lastFrame = 0.0f; // Time of last frame

/* Camera */
Camera camera(vec3(0.0f, 0.0f, 3.0f));
bool firstMouse = true;
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;

/* Texture */
int texType = 1;


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
    GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Learn OpenGL", nullptr, nullptr);
    if (window == nullptr) {
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

    /* Configure global state */
    stbi_set_flip_vertically_on_load(true);
    glEnable(GL_DEPTH_TEST);

    /* Shader Settings */
    const string vertexPath = "include/shaders/shader.vs";
    const string fragmentPath = "include/shaders/shader.fs";
    Shader ourShader(vertexPath.c_str(), fragmentPath.c_str());

    /* Model Loading */
    const string &modelPath = "model/backpack/backpack.obj";
    Model ourModel(modelPath);

    /* Render Loop */
    while (!glfwWindowShouldClose(window)) {
        // Per-Frame time logic
        auto currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(window);

        // Render
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Enable shader before setting uniforms
        ourShader.use();

        // Pass texture type to the fragment shader
        ourShader.setInt("texture_type", texType);

        // Create transformations
        // Pass projection matrix to shader (Note that in this case it could change every frame)
        mat4 projection = perspective(radians(camera.Zoom), (float) SCR_WIDTH / (float) SCR_HEIGHT, 0.1f, 100.0f);
        ourShader.setMat4("projection", projection);

        // Camera / View transformation
        mat4 view = camera.GetViewMatrix();
        ourShader.setMat4("view", view);

        // Render the loaded model
        mat4 model = mat4(1.0f);
        model = translate(model, vec3(0.0f, 0.0f, 0.0f)); // Translate the center of the scene
        model = scale(model, vec3(1.0f, 1.0f, 1.0f));

        // Pass locations to the shaders
        ourShader.setMat4("model", model);
        ourModel.Draw(ourShader);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

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

    // Camera input
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

    // Texture input
    if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) {
        texType = 1;
    }
    if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS) {
        texType = 2;
    }
    if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS) {
        texType = 3;
    }
    if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS) {
        texType = 4;
    }
}

void mouse_callback(GLFWwindow *window, double xposIn, double yposIn) {
    auto xpos = static_cast<float>(xposIn);
    auto ypos = static_cast<float>(yposIn);

    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates range from bottom to top
    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow *window, double xoffset, double yoffset) {
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}
