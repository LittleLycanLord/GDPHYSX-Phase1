#pragma once
#include "../../stb_image.h"
#include "glad/glad.h"
#include "stdafx.h"

namespace rendering {
class MyTexture {
    //* ╔════════════╗
    //* ║ Attributes ║
    //* ╚════════════╝
protected:
    GLuint texture;
    std::string texturePath;
    int width;
    int height;
    int colorChannels;

    //* ╔═══════════════════════════════╗
    //* ║ Constructors & Deconstructors ║
    //* ╚═══════════════════════════════╝
public:
    MyTexture(std::string texturePath);

    //* ╔═════════╗
    //* ║ Methods ║
    //* ╚═════════╝
public:
    void loadTexture();

    //* ╔═══════════════════╗
    //* ║ Getters & Setters ║
    //* ╚═══════════════════╝
public:
    GLuint getTexture();
    void setTexture(GLuint texture);
    std::string getTexturePath();
    void setTexturePath(std::string texturePath);
    int getWidth();
    void setWidth(int width);
    int getHeight();
    void setHeight(int height);
    int getColorChannels();
    void setColorChannels(int colorChannels);
    int getTextureCount();
};
}  // namespace rendering