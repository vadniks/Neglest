
#pragma once

#include <cglm/types.h>

struct CompoundShader;
typedef struct CompoundShader CompoundShader;

CompoundShader* compoundShaderCreate(const char* vertexPath, const char* fragmentPath);
void compoundShaderDestroy(CompoundShader* shader);
void compoundShaderUse(const CompoundShader* shader);
void compoundShaderSetBool(const CompoundShader* shader, const char* name, bool value);
void compoundShaderSetInt(const CompoundShader* shader, const char* name, int value);
void compoundShaderSetFloat(const CompoundShader* shader, const char* name, float value);
void compoundShaderSetVec2(const CompoundShader* shader, const char* name, const vec2 value);
void compoundShaderSetVec3(const CompoundShader* shader, const char* name, const vec3 value);
void compoundShaderSetVec4(const CompoundShader* shader, const char* name, const vec4 value);
void compoundShaderSetMat3(const CompoundShader* shader, const char* name, const mat3 value);
void compoundShaderSetMat4(const CompoundShader* shader, const char* name, const mat4 value);

#define compoundShaderSetPrimitive(s, n, v) _Generic( \
    v, \
    bool: compoundShaderSetBool, \
    int: compoundShaderSetInt, \
    float: compoundShaderSetFloat \
)(v) // arrays don't work here
