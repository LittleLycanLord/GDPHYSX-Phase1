#pragma once
#include "../MyForceGenerator.hpp"
#include "stdafx.h"

namespace MyPhysics {

using namespace std;

class MyDragGenerator : public MyForceGenerator {
    //* ╔════════════╗
    //* ║ Attributes ║
    //* ╚════════════╝
protected:
    double dragCoefficient1 = DEFAULT_DRAG_COEFFICIENT_1;
    double dragCoefficient2 = DEFAULT_DRAG_COEFFICIENT_2;

    //* ╔═══════════════════════════════╗
    //* ║ Constructors & Deconstructors ║
    //* ╚═══════════════════════════════╝
public:
    MyDragGenerator();
    MyDragGenerator(double dragCoefficient1, double dragCoefficient2);

    //* ╔═════════╗
    //* ║ Methods ║
    //* ╚═════════╝
public:
    virtual void updateForce(MyParticle* targetParticle, double time) override;

private:
    //* ╔═══════════════════╗
    //* ║ Getters & Setters ║
    //* ╚═══════════════════╝
public:
};
}  // namespace MyPhysics