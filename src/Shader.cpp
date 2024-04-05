
#include "Shader.hpp"
#include "defs.hpp"
#include <fstream>
#include <sstream>
#include <GL/glew.h>

static void readShaderFiles(
    const std::string& vertexShaderFilePath,
    const std::string& fragmentShaderFilePath,
    std::string& vertexShaderCode,
    std::string& fragmentShaderCode
) { try {
    std::fstream vertexShaderFile, fragmentShaderFile;
    std::stringstream vertexShaderStream, fragmentShaderStream;

    vertexShaderFile.open(vertexShaderFilePath);
    fragmentShaderFile.open(fragmentShaderFilePath);

    vertexShaderStream << vertexShaderFile.rdbuf();
    fragmentShaderStream << fragmentShaderFile.rdbuf();

    vertexShaderFile.close();
    fragmentShaderFile.close();

    vertexShaderCode = vertexShaderStream.str();
    fragmentShaderCode = fragmentShaderStream.str();
} catch (...) { assert(false); } }

static unsigned init(const std::string& vertexPath, const std::string& fragmentPath) {
    std::string vertexShaderCode, fragmentShaderCode;
    readShaderFiles(vertexPath, fragmentPath, vertexShaderCode, fragmentShaderCode);

    const char* vertexCode, * fragmentCode;
    unsigned vertex, fragment;
    int success;

    vertexCode = vertexShaderCode.c_str();
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vertexCode, nullptr);
    glCompileShader(vertex);
    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    assert(success == 1);

    fragmentCode = fragmentShaderCode.c_str();
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fragmentCode, nullptr);
    glCompileShader(fragment);
    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    assert(success == 1);

    unsigned id = glCreateProgram();
    glAttachShader(id, vertex);
    glAttachShader(id, fragment);
    glLinkProgram(id);
    glGetProgramiv(id, GL_LINK_STATUS, &success);
    assert(success == 1);

    glDeleteShader(vertex);
    glDeleteShader(fragment);

    return id;
}

Shader::Shader(const std::string& vertexPath, const std::string& fragmentPath)
    : id(init(vertexPath, fragmentPath))
{}

Shader::~Shader() { glDeleteProgram(id); }

void Shader::use() const { glUseProgram(id); }

void Shader::setValue(const std::string& name, bool value) const
{ setValue(name, static_cast<int>(value)); }

void Shader::setValue(const std::string& name, int value) const
{ glUniform1i(glGetUniformLocation(id, name.c_str()), value); }

void Shader::setValue(const std::string& name, float value) const
{ glUniform1f(glGetUniformLocation(id, name.c_str()), value); }
