#pragma once

#include <glad/glad.h>
#include "VBO.hpp"

class VAO{
    public:
        GLuint id;
        void Bind(){
           glBindVertexArray(id);
        }
        static void UnBind(){
            glBindVertexArray(0);
        }
        void Delete(){
            glDeleteVertexArrays(1, &id);
        }

        void LinkVBO(VBO &vbo, GLuint layout,GLuint components_count, GLenum type, GLsizei stride_size, void *offset){
            Bind();
            vbo.Bind();
            glVertexAttribPointer(layout, components_count, type, GL_FALSE, stride_size, offset);
            glEnableVertexAttribArray(layout);
        }

        VAO(){
            glGenVertexArrays(1, &id);
        }
};