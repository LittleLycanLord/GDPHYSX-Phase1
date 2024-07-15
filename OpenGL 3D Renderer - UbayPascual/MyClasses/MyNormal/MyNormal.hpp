#pragma once
#include "../../stb_image.h"
#include "glad/glad.h"
#include "stdafx.h"

namespace rendering {
class MyNormal {
    //* ╔════════════╗
    //* ║ Attributes ║
    //* ╚════════════╝
protected:
    GLuint normal;
    std::string normalPath;
    int width;
    int height;
    int colorChannels;

    //* ╔═══════════════════════════════╗
    //* ║ Constructors & Deconstructors ║
    //* ╚═══════════════════════════════╝
public:
    MyNormal(std::string normalPath);

    //* ╔═════════╗
    //* ║ Methods ║
    //* ╚═════════╝
public:
    void loadTexture();
    void loadNormal();

    //* ╔═══════════════════╗
    //* ║ Getters & Setters ║
    //* ╚═══════════════════╝
public:
    GLuint getNormal();
    void setNormal(GLuint normal);
    std::string getNormalPath();
    void setNormalPath(std::string normalPath);
    int getWidth();
    void setWidth(int width);
    int getHeight();
    void setHeight(int height);
    int getColorChannels();
    void setColorChannels(int colorChannels);
};
}  // namespace rendering