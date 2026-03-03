#pragma once

#include <glad/glad.h>

class EBO{
    public: 
        GLuint id;

        void Bind(){
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
        }

        static void UnBind(){
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        }

        void Delete(){
            glDeleteBuffers(1, &id);
        }

        EBO(GLuint *indices, GLsizei indices_size, GLenum usage){
            glGenBuffers(1, &id);
            Bind();
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices_size, indices, usage);
        }
};