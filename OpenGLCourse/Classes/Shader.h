#pragma once

#include <string>
#include <iostream>
#include <GL/glew.h>

#pragma once
class Shader
{
public:
    Shader();
    void CreateFromFiles(const char* vShader, const char* fShader);
    void UseShader() { glUseProgram(this->shader); }
    GLuint GetModelLocation() { return this->uniformModel; }
    GLuint GetProjectionLocation() { return this->uniformProjection; }
    GLuint GetViewLocation() {return  this->uniformView;}
    ~Shader();
private:
    GLuint shader, uniformModel, uniformProjection, uniformView;
    std::string readShaderCodeFromFile(const char* shaderPath);
    void addShader(GLuint theProgram, const char* shaderCode, GLenum shaderType);
    void compileShaders(const char* vShaderCode, const char* fShaderCode);
};
