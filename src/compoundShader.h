/*
 * Neglest - an OpenGL 2D game.
 * Copyright (C) 2024  Vadim Nikolaev (https://github.com/vadniks).
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
