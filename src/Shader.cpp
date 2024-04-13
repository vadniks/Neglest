
#include "Shader.hpp"
#include <GL/glew.h>
#include <glm/gtc/type_ptr.hpp>

static void assertCompiledOrLinked(unsigned id, bool compiled) {
    int success;
    compiled
        ? glGetShaderiv(id, GL_COMPILE_STATUS, &success)
        : glGetProgramiv(id, GL_LINK_STATUS, &success);
    assert(success == GL_TRUE);
}

Shader::Shader(const std::string& vertexSource, const std::string& fragmentSource) {
    const unsigned vertexId = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexId, 1, (const char*[1]) {vertexSource.c_str()}, nullptr);
    glCompileShader(vertexId);
    assertCompiledOrLinked(vertexId, true);

    const unsigned fragmentId = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentId, 1, (const char*[1]) {fragmentSource.c_str()}, nullptr);
    glCompileShader(fragmentId);
    assertCompiledOrLinked(fragmentId, true);

    mId = glCreateProgram();
    glAttachShader(mId, vertexId);
    glAttachShader(mId, fragmentId);
    glLinkProgram(mId);
    assertCompiledOrLinked(mId, false);

    glDeleteShader(vertexId);
    glDeleteShader(fragmentId);
}

Shader::~Shader() {
    glDeleteProgram(mId);
}

unsigned Shader::id() {
    return mId;
}

void Shader::use() {
    glUseProgram(mId);
}

void Shader::setValue(const std::string& name, float value) {
    glUniform1f(glGetUniformLocation(mId, name.c_str()), value);
}

void Shader::setValue(const std::string& name, int value) {
    glUniform1i(glGetUniformLocation(mId, name.c_str()), value);
}

void Shader::setValue(const std::string& name, const glm::vec2& value) {
    glUniform2f(glGetUniformLocation(mId, name.c_str()), value.x, value.y);
}

void Shader::setValue(const std::string& name, const glm::vec3& value) {
    glUniform3f(glGetUniformLocation(mId, name.c_str()), value.x, value.y, value.z);
}

void Shader::setValue(const std::string& name, const glm::vec4& value) {
    glUniform4f(glGetUniformLocation(mId, name.c_str()), value.x, value.y, value.z, value.w);
}

void Shader::setValue(const std::string& name, const glm::mat4& value) {
    glUniformMatrix4fv(glGetUniformLocation(mId, name.c_str()), 1, false, glm::value_ptr(value));
}
