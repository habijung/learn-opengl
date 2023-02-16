//
// Created by habi on 10/24/2022.
//
#ifndef SHADER_H
#define SHADER_H

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include "glad/glad.h"
#include "glm/glm.hpp"

using namespace std;


/* Shader Class */
class Shader {
public:
    // the program ID
    unsigned int ID;

    // constructor reads and builds the shader
    Shader(const char *vertexPath, const char *fragmentPath) {
        // 1. Vertex/Fragment 코드를 filePath에서 가져오기
        string vertexCode;
        string fragmentCode;
        ifstream vShaderFile;
        ifstream fShaderFile;

        // ifstream이 문제는 없는지 exception 확인
        vShaderFile.exceptions(ifstream::failbit | ifstream::badbit);
        fShaderFile.exceptions(ifstream::failbit | ifstream::badbit);

        try {
            // File open
            vShaderFile.open(vertexPath);
            fShaderFile.open(fragmentPath);

            stringstream vShaderStream, fShaderStream;
            vShaderStream << vShaderFile.rdbuf();
            fShaderStream << fShaderFile.rdbuf();
            vShaderFile.close();
            fShaderFile.close();
            vertexCode = vShaderStream.str();
            fragmentCode = fShaderStream.str();
        } catch (ifstream::failure &e) {
            cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ: " << e.what() << endl;
        }

        const char *vShaderCode = vertexCode.c_str();
        const char *fShaderCode = fragmentCode.c_str();

        // 2. compile shaders
        unsigned int vertex, fragment;

        // Vertex Shader
        vertex = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex, 1, &vShaderCode, NULL);
        glCompileShader(vertex);
        checkCompileErrors(vertex, "VERTEX");// Vertex Shader compile 성공 확인

        // Fragment Shader
        fragment = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment, 1, &fShaderCode, NULL);
        glCompileShader(fragment);
        checkCompileErrors(fragment, "FRAGMENT");// Fragment Shader compile 성공 확인

        // Shader Program
        ID = glCreateProgram();
        glAttachShader(ID, vertex);
        glAttachShader(ID, fragment);
        glLinkProgram(ID);
        checkCompileErrors(ID, "PROGRAM");// Shader program compile 성공 확인

        // Shader Source 삭제
        glDeleteShader(vertex);
        glDeleteShader(fragment);
    };

    // use/activate the shader
    void use() {
        glUseProgram(ID);
    };

    // utility uniform functions
    void setBool(const string &name, bool value) const {
        glUniform1i(glGetUniformLocation(ID, name.c_str()), (int) value);
    };

    void setInt(const string &name, int value) const {
        glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
    };

    void setFloat(const string &name, float value) const {
        glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
    };

    void setMat4(const string &name, const glm::mat4 &mat) const {
        glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    };

    void setVec3(const string &name, const glm::vec3 &vec) const {
        glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &vec[0]);
    };

private:
    static void checkCompileErrors(GLuint shader, string type) {
        int success;
        char infoLog[1024];

        if (type != "PROGRAM") {
            glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
            if (!success) {
                glGetShaderInfoLog(shader, 1024, NULL, infoLog);
                cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n"
                     << infoLog << "\n\n"
                     << endl;
            } else {
                glGetProgramiv(shader, GL_LINK_STATUS, &success);
                if (!success) {
                    glGetProgramInfoLog(shader, 1024, NULL, infoLog);
                    cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n"
                         << infoLog << "\n\n"
                         << endl;
                }
            }
        }
    }
};

#endif
