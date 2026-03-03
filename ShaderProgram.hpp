#pragma once

#include <glad/glad.h>
#include <fstream>
#include <sstream>
#include <string>

class ShaderProgram{
    public:
        GLuint id;

        void Activate(){
            glUseProgram(id);
        }

        void Delete(){
            glDeleteProgram(id);
        }

        ShaderProgram(const char *vrtx_shader_source_path, const char *frgmnt_shader_source_path){
            std::ifstream vertexShaderFile(vrtx_shader_source_path);
            std::ifstream fragmentShaderFile(frgmnt_shader_source_path);

            std::stringstream vertexShaderSourceStream;
            std::stringstream fragmentShaderSourceStream;

            vertexShaderSourceStream << vertexShaderFile.rdbuf();
            fragmentShaderSourceStream << fragmentShaderFile.rdbuf();

            std::string vertexShaderSourceString = vertexShaderSourceStream.str();
            std::string fragmentShaderSourceString = fragmentShaderSourceStream.str();

            const char *vertexShaderSource = vertexShaderSourceString.c_str();
            const char *fragmentShaderSource = fragmentShaderSourceString.c_str();

            GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
            glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
            glCompileShader(vertexShader);

            GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
            glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
            glCompileShader(fragmentShader);

            id = glCreateProgram();
            glAttachShader(id, vertexShader);
            glAttachShader(id, fragmentShader);
            glLinkProgram(id);

            glDeleteShader(vertexShader);
            glDeleteShader(fragmentShader);
        }
};