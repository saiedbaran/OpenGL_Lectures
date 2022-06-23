#include "Shader.h"

#include <fstream>
#include <sstream>

Shader::Shader()
{
    shader = 0, uniformModel = 0, uniformProjection = 0;
}

void Shader::CreateFromFiles(const char* vShader, const char* fShader)
{
    std::string vShaderCode = readShaderCodeFromFile(vShader);
    std::string fShaderCode = readShaderCodeFromFile(fShader);

    const char* vertexCode = vShaderCode.c_str();
    const char* fragmentCode = fShaderCode.c_str();

    compileShaders(vertexCode, fragmentCode);
}

Shader::~Shader()
{
}

std::string Shader::readShaderCodeFromFile(const char* shaderPath)
{
    std::string code;
    std::ifstream fileStream(shaderPath, std::ios::in);
    if (!fileStream.is_open()) {
        printf("Failed to read %s! File doesn't exist.", shaderPath);
        return "";
    }

    std::string line = "";
    while (!fileStream.eof())
    {
        std::getline(fileStream, line);
        code.append(line + "\n");
    }

    fileStream.close();
    return code;
}

void Shader::addShader(GLuint theProgram, const char* shaderCode, GLenum shaderType)
{
    GLuint theShader = glCreateShader(shaderType);

    const GLchar* theCode[1];
    theCode[0] = shaderCode;

    GLint codeLength[1];
    codeLength[0] = strlen(shaderCode);

    glShaderSource(theShader, 1, theCode, codeLength);
    glCompileShader(theShader);

    GLint result = 0;
    GLchar errLog[1024] = { 0 };

    glGetShaderiv(theShader, GL_COMPILE_STATUS, &result);

    if (!result)
    {
        glGetShaderInfoLog(theShader, sizeof(errLog), NULL, errLog);
        std::cerr << "Error compiling the " << shaderType << "shader:" << errLog << "\n";
        return;
    }

    glAttachShader(theProgram, theShader);
}

void Shader::compileShaders(const char* vShaderCode, const char* fShaderCode)
{
    shader = glCreateProgram();

    if (!shader) {
        std::cerr << "Error creating shader program\n";
        return;
    }

    addShader(shader, vShaderCode, GL_VERTEX_SHADER);
    addShader(shader, fShaderCode, GL_FRAGMENT_SHADER);

    GLint result = 0;
    GLchar errLog[1024] = { 0 };

    glLinkProgram(shader);
    glGetProgramiv(shader, GL_LINK_STATUS, &result);

    if (!result)
    {
        glGetProgramInfoLog(shader, sizeof(errLog), NULL, errLog);
        std::cerr << "Error linking program:" << errLog << "\n";
        return;
    }

    glValidateProgram(shader);
    glGetProgramiv(shader, GL_VALIDATE_STATUS, &result);

    if (!result)
    {
        glGetProgramInfoLog(shader, sizeof(errLog), NULL, errLog);
        std::cerr << "Error validating program:" << errLog << "\n";
        return;
    }

    uniformModel = glGetUniformLocation(shader, "model");
    uniformProjection = glGetUniformLocation(shader, "projection");
    uniformView = glGetUniformLocation(shader, "view");
}
