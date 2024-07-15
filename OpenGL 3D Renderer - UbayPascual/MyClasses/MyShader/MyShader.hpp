#pragma once

#include "Enum/ShaderType.hpp"
#include "glad/glad.h"
#include "stdafx.h"

using namespace std;

namespace rendering {
class MyShader {
    //* ╔════════════╗
    //* ║ Attributes ║
    //* ╚════════════╝
protected:
    string path;
    ShaderType type;

    fstream file;
    stringstream buffer;
    string shaderString;
    const char* charArray;
    GLuint shader;

    //* ╔═══════════════════════════════╗
    //* ║ Constructors & Deconstructors ║
    //* ╚═══════════════════════════════╝
public:
    MyShader(std::string path, ShaderType type);

    //* ╔═════════╗
    //* ║ Methods ║
    //* ╚═════════╝
public:
    void loadShader();

    //* ╔═══════════════════╗
    //* ║ Getters & Setters ║
    //* ╚═══════════════════╝
    GLuint getShader();

public:
};
}  // namespace rendering