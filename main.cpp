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
     */
    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // 프로그램이 정지 신호를 받기 전까지 계속 image render
    while (!glfwWindowShouldClose(window))
    {
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
