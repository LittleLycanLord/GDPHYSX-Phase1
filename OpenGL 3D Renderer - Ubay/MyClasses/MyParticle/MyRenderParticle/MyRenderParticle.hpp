#pragma once
#include "../../My3DModel/My3DModel.hpp"
#include "../MyParticle.hpp"
#include "stdafx.h"

using namespace rendering;

namespace MyPhysics {
class MyRenderParticle : public MyParticle {
    //* ╔════════════╗
    //* ║ Attributes ║
    //* ╚════════════╝
protected:
    My3DModel* model;
    MyVector3 tint;

    //* ╔═══════════════════════════════╗
    //* ║ Constructors & Deconstructors ║
    //* ╚═══════════════════════════════╝
public:
    // Default Model with Custom Color
    MyRenderParticle(My3DModel* model, MyVector3 tint);
    MyRenderParticle(My3DModel* model, double mass, MyVector3 tint);
    MyRenderParticle(My3DModel* model, double mass, MyVector3 position, MyVector3 tint);

    //* ╔═════════╗
    //* ║ Methods ║
    //* ╚═════════╝
protected:
    void updateModel();
    //* ╔═══════════════════╗
    //* ║ Getters & Setters ║
    //* ╚═══════════════════╝
public:
    My3DModel* getModel3D();
    MyVector3 getColor();
    void setColor(MyVector3 tint);
};
}  // namespace MyPhysics