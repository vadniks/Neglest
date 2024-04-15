
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
    const char* const xVertexSource = vertexSource.c_str();
    const char* const xFragmentSource = fragmentSource.c_str();

    const unsigned vertexId = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexId, 1, &xVertexSource, nullptr);
    glCompileShader(vertexId);
    assertCompiledOrLinked(vertexId, true);

    const unsigned fragmentId = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentId, 1, &xFragmentSource, nullptr);
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

unsigned Shader::id() const {
    return mId;
}

void Shader::use() const {
    glUseProgram(mId);
}

void Shader::setValue(const std::string& name, float value) const {
    glUniform1f(glGetUniformLocation(mId, name.c_str()), value);
}

void Shader::setValue(const std::string& name, int value) const {
    glUniform1i(glGetUniformLocation(mId, name.c_str()), value);
}

void Shader::setValue(const std::string& name, const glm::vec2& value) const {
    glUniform2f(glGetUniformLocation(mId, name.c_str()), value.x, value.y);
}

void Shader::setValue(const std::string& name, const glm::vec3& value) const {
    glUniform3f(glGetUniformLocation(mId, name.c_str()), value.x, value.y, value.z);
}

void Shader::setValue(const std::string& name, const glm::vec4& value) const {
    glUniform4f(glGetUniformLocation(mId, name.c_str()), value.x, value.y, value.z, value.w);
}

void Shader::setValue(const std::string& name, const glm::mat4& value) const {
    glUniformMatrix4fv(glGetUniformLocation(mId, name.c_str()), 1, false, glm::value_ptr(value));
}
