
#pragma once

#include <string>
#include <glm/glm.hpp>

class CompoundShader final {
public:
    const unsigned id;
public:
    CompoundShader(const std::string& vertexPath, const std::string& fragmentPath);
    CompoundShader(const CompoundShader&) = delete;
    ~CompoundShader();
    void use() const;
    void setValue(const std::string& name, bool value) const;
    void setValue(const std::string& name, int value) const;
    void setValue(const std::string& name, float value) const;
    void setValue(const std::string& name, const glm::mat3& value) const;
    void setValue(const std::string& name, const glm::mat4& value) const;
};
