#pragma once
#include "../MyForceGenerator.hpp"
#include "stdafx.h"

namespace MyPhysics {

using namespace std;

class MyGravityGenerator : public MyForceGenerator {
    //* ╔════════════╗
    //* ║ Attributes ║
    //* ╚════════════╝
protected:
    MyVector3 gravity = MyVector3(DEFAULT_GRAVITY_X, DEFAULT_GRAVITY_Y, DEFAULT_GRAVITY_Z);

    //* ╔═══════════════════════════════╗
    //* ║ Constructors & Deconstructors ║
    //* ╚═══════════════════════════════╝
public:
    MyGravityGenerator();
    MyGravityGenerator(MyVector3 gravity);

    //* ╔═════════╗
    //* ║ Methods ║
    //* ╚═════════╝
public:
    void updateForce(MyParticle* targetParticle, double time) override;

private:
    //* ╔═══════════════════╗
    //* ║ Getters & Setters ║
    //* ╚═══════════════════╝
public:
};
}  // namespace MyPhysics