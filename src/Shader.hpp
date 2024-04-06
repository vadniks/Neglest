
#pragma once

#include <string>
#include <glm/glm.hpp>

class Shader final {
public:
    const unsigned id;
public:
    Shader(const std::string& vertexPath, const std::string& fragmentPath);
    Shader(const Shader&) = delete;
    ~Shader();
    void use() const;
    void setValue(const std::string& name, bool value) const;
    void setValue(const std::string& name, int value) const;
    void setValue(const std::string& name, float value) const;
    void setValue(const std::string& name, const glm::mat3& value) const;
    void setValue(const std::string& name, const glm::mat4& value) const;
};
