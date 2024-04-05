/*
 * Neglest - An OpenGL 3D game.
 * Copyright (C) 2024 Vadim Nikolaev (https://github.com/vadniks).
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "Shader.hpp"
#include "defs.hpp"
#include <fstream>
#include <sstream>
#include <GL/glew.h>
#include <glm/gtc/type_ptr.hpp>

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

void Shader::setValue(const std::string& name, const glm::mat3& value) const
{ glUniformMatrix3fv(glGetUniformLocation(id, name.c_str()), 1, GL_FALSE, glm::value_ptr(value)); }

void Shader::setValue(const std::string& name, const glm::mat4& value) const
{ glUniformMatrix4fv(glGetUniformLocation(id, name.c_str()), 1, GL_FALSE, glm::value_ptr(value)); }
