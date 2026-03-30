#define GLEW_STATIC
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <cmath>
#include "glew-2.1.0/include/GL/glew.h"
#include "glfw-3.4.bin.WIN64/include/GLFW/glfw3.h"

//2 задание вметодичке
std::string readShader(const char* filePath) {
    std::string content;
    std::ifstream fileStream(filePath, std::ios::in);
    if (!fileStream.is_open()) {
        std::cerr << "Не читается " << filePath << ". НЕ выходит" << std::endl;
        return "";
    }
    std::stringstream sstr;
    sstr << fileStream.rdbuf();
    content = sstr.str();
    fileStream.close();
    return content;
}

int main() {
    if (!glfwInit()) return -1;

    // 4 6 версия
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(512, 512, "Lab 2 - Variant 4", NULL, NULL);
    if (!window) { glfwTerminate(); return -1; }
    glfwMakeContextCurrent(window);

    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) return -1;

    std::string vertSrc = readShader("shaders/vertex.glsl");
    std::string fragSrc = readShader("shaders/fragment.glsl");
    const char* vShaderCode = vertSrc.c_str();
    const char* fShaderCode = fragSrc.c_str();

    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vShaderCode, NULL);
    glCompileShader(vertexShader);

    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fShaderCode, NULL);
    glCompileShader(fragmentShader);

    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    float vertices[] = {
         0.5f,  0.5f, 0.0f,  //верх право
         0.5f, -0.5f, 0.0f,  //низ право
        -0.5f, -0.5f, 0.0f,  //низ лево
        -0.5f,  0.5f, 0.0f   //верх лево
    };
    unsigned int indices[] = {
        0, 1, 3, 
        1, 2, 3  
    };

    //VBO VAO EBO
    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    while (!glfwWindowShouldClose(window)) {
        glClearColor(0.5f, 0.5f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);

        // движение 
        float timeValue = glfwGetTime();
        float xOffset = sin(timeValue) * 0.4f; 
        int xOffsetLocation = glGetUniformLocation(shaderProgram, "xOffset");
        glUniform1f(xOffsetLocation, xOffset);

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glfwTerminate();
    return 0;
}