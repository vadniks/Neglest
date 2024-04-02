
#pragma once

#include <string>

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
};
