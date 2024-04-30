
#include "compoundShader.h"
#include "defs.h"
#include <SDL2/SDL.h>
#include <GL/glew.h>

struct CompoundShader {
    unsigned programId;
};

CompoundShader* compoundShaderCreate(const char* vertexPath, const char* fragmentPath) {
    SDL_RWops* vertexFile = SDL_RWFromFile(vertexPath, "r"); // TODO: it's a mess
    assert(vertexFile != nullptr);
    const unsigned vertexSize = SDL_RWsize(vertexFile);
    char vertexCode[vertexSize + 1];
    assert(SDL_RWread(vertexFile, vertexCode, 1, vertexSize) == vertexSize);
    vertexCode[vertexSize] = 0;
    SDL_RWclose(vertexFile);

    SDL_RWops* fragmentFile = SDL_RWFromFile(fragmentPath, "r"); // TODO: it's a mess
    assert(fragmentFile != nullptr);
    const unsigned fragmentSize = SDL_RWsize(fragmentFile);
    char fragmentCode[fragmentSize + 1];
    assert(SDL_RWread(fragmentFile, fragmentCode, 1, fragmentSize) == fragmentSize);
    fragmentCode[fragmentSize] = 0;
    SDL_RWclose(fragmentFile);

    CompoundShader* shader = SDL_malloc(sizeof *shader);
    int success;

    unsigned vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, (const char*[1]) {vertexCode}, nullptr);
    glCompileShader(vertex);
    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    assert(success == GL_TRUE);

    unsigned fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, (const char*[1]) {fragmentCode}, nullptr);
    glCompileShader(fragment);
    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    assert(success == GL_TRUE);

    shader->programId = glCreateProgram();
    glAttachShader(shader->programId, vertex);
    glAttachShader(shader->programId, fragment);
    glLinkProgram(shader->programId);
    glGetProgramiv(shader->programId, GL_LINK_STATUS, &success);
    assert(success == GL_TRUE);

    glDeleteShader(vertex);
    glDeleteShader(fragment);

    return shader;
}

void compoundShaderDestroy(CompoundShader* shader) {
    glDeleteProgram(shader->programId);
    SDL_free(shader);
}

void compoundShaderUse(const CompoundShader* shader)
{ glUseProgram(shader->programId); }

void compoundShaderSetBool(const CompoundShader* shader, const char* name, bool value)
{ glUniform1i(glGetUniformLocation(shader->programId, name), (int) value); }

void compoundShaderSetInt(const CompoundShader* shader, const char* name, int value)
{ glUniform1i(glGetUniformLocation(shader->programId, name), value); }

void compoundShaderSetFloat(const CompoundShader* shader, const char* name, float value)
{ glUniform1f(glGetUniformLocation(shader->programId, name), value); }

void compoundShaderSetVec2(const CompoundShader* shader, const char* name, const vec2 value)
{ glUniform2f(glGetUniformLocation(shader->programId, name), value[0], value[1]); }

void compoundShaderSetVec3(const CompoundShader* shader, const char* name, const vec3 value)
{ glUniform3f(glGetUniformLocation(shader->programId, name), value[0], value[1], value[2]); }

void compoundShaderSetVec4(const CompoundShader* shader, const char* name, const vec4 value)
{ glUniform4f(glGetUniformLocation(shader->programId, name), value[0], value[1], value[2], value[3]); }

void compoundShaderSetMat3(const CompoundShader* shader, const char* name, const mat3 value)
{ glUniformMatrix3fv(glGetUniformLocation(shader->programId, name), 1, GL_FALSE, &(value[0][0])); }

void compoundShaderSetMat4(const CompoundShader* shader, const char* name, const mat4 value)
{ glUniformMatrix4fv(glGetUniformLocation(shader->programId, name), 1, GL_FALSE, &(value[0][0])); }
