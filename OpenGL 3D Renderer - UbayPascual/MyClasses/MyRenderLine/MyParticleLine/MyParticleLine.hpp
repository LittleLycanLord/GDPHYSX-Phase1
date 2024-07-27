#pragma once
#include "../../MyParticle/MyParticle.hpp"
#include "../../MyVectors/MyVector3.hpp"
#include "../MyRenderLine.hpp"
#include "glad/glad.h"
#include "stdafx.h"

using namespace MyPhysics;

namespace rendering {
class MyParticleLine : public MyRenderLine {
    //* ╔════════════╗
    //* ║ Attributes ║
    //* ╚════════════╝
private:
    MyParticle* particleA;
    MyParticle* particleB;

    //* ╔═══════════════════════════════╗
    //* ║ Constructors & Deconstructors ║
    //* ╚═══════════════════════════════╝
public:
    MyParticleLine(MyParticle* particleA, MyParticle* particleB);
    MyParticleLine(MyParticle* particleA, MyParticle* particleB, MyVector3 color);

    //* ╔═════════╗
    //* ║ Methods ║
    //* ╚═════════╝
public:
    void update(mat4 projectionMatrix);

    //* ╔═══════════════════╗
    //* ║ Getters & Setters ║
    //* ╚═══════════════════╝

public:
};
}  // namespace rendering