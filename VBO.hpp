#pragma once

#include <glad/glad.h>

class VBO{
    public: 
        GLuint id;

        void Bind(){
            glBindBuffer(GL_ARRAY_BUFFER, id);
        }

        static void UnBind(){
            glBindBuffer(GL_ARRAY_BUFFER, 0);
        }

        void Delete(){
            glDeleteBuffers(1, &id);
        }

        VBO(GLfloat *vertices, GLsizei vertices_size, GLenum usage){
            glGenBuffers(1, &id);
            Bind();
            glBufferData(GL_ARRAY_BUFFER, vertices_size, vertices, usage);
        }
};