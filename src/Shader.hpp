
#pragma once

#include <string>
#include <glm/glm.hpp>

class Shader {
private:
    unsigned mId;
public:
    Shader(const std::string& vertexSource, const std::string& fragmentSource);
    ~Shader();
    [[nodiscard]] unsigned id() const;
    void use() const;
    void setValue(const std::string& name, float value) const;
    void setValue(const std::string& name, int value) const;
    void setValue(const std::string& name, const glm::vec2& value) const;
    void setValue(const std::string& name, const glm::vec3& value) const;
    void setValue(const std::string& name, const glm::vec4& value) const;
    void setValue(const std::string& name, const glm::mat4& value) const;
};
