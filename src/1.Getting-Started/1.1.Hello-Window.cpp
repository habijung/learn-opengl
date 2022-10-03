//
// Created by habi on 9/6/2022.
//
/*
 * GLFW 보다 GLAD를 먼저 include 해야함.
 * 뒤에서 필요한 OpenGL header 파일이 GLAD에 포함되기 때문임.
 */
#include <iostream>
#include "glad/glad.h"
#include "GLFW/glfw3.h"


/* Prototypes */
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);


/* Main */
int main()
{
    std::cout << "Run main()" << std::endl;

    // GLFW 초기화
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    // Window object 생성
    GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to creat GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // Initialize GLAD
    /*
     * macOS에서 error 발생함.
     * Error: symbol(s) not found for architecture arm64
     * CMakeLists에서 GLAD가 executable taget으로 연결되어 있지 않아서 발생하는 문제
     */
    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // 프로그램이 정지 신호를 받기 전까지 계속 image render하는 loop
    while (!glfwWindowShouldClose(window))
    {
        // Input
        processInput(window);

        // Rendering commands here
        // 실제로 작동하는지 확인을 위해 원하는 색상으로 화면 clear
        // Clear하는 것은 render loop에서 실행되고 다시 로딩할 때, 덮어쓰기로 현재는 이해함.
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Check and all events and swap the buffers
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();

    return 0;
}


/* Functions */
// 사용자가 윈도우 크기를 조정할 때, 뷰포트도 조정하는 callback function
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

// Key input에 따른 반응 function
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}
