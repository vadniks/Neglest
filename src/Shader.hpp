
#pragma once

#include <string>
#include <glm/glm.hpp>

class Shader {
private:
    unsigned mId;
public:
    Shader(const std::string& vertexSource, const std::string& fragmentSource);
    ~Shader();
    unsigned id();
    void use();
    void setValue(const std::string& name, float value);
    void setValue(const std::string& name, int value);
    void setValue(const std::string& name, const glm::vec2& value);
    void setValue(const std::string& name, const glm::vec3& value);
    void setValue(const std::string& name, const glm::vec4& value);
    void setValue(const std::string& name, const glm::mat4& value);
};
