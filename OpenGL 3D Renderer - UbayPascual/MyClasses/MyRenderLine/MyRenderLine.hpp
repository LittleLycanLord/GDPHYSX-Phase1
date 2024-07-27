#pragma once
#include "../MyVectors/MyVector3.hpp"
#include "glad/glad.h"
#include "stdafx.h"

using namespace std;
using namespace glm;
using namespace MyPhysics;

namespace rendering {
class MyRenderLine {
    //* ╔════════════╗
    //* ║ Attributes ║
    //* ╚════════════╝
protected:
    MyVector3 positionA;
    MyVector3 positionB;
    mat4 projectionMatrix;
    MyVector3 color;

    //* ╔═══════════════════════════════╗
    //* ║ Constructors & Deconstructors ║
    //* ╚═══════════════════════════════╝
public:
    MyRenderLine(MyVector3 positionA, MyVector3 positionB);
    MyRenderLine(MyVector3 positionA, MyVector3 positionB, MyVector3 color);

    //* ╔═════════╗
    //* ║ Methods ║
    //* ╚═════════╝
public:
    void update(MyVector3 positionA, MyVector3 positionB, mat4 projectionMatrix);
    virtual void update(mat4 projectionMatrix);
    void draw();

    //* ╔═══════════════════╗
    //* ║ Getters & Setters ║
    //* ╚═══════════════════╝

public:
};
}  // namespace rendering