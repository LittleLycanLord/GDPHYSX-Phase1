#pragma once
#include "../MyParticle/MyParticle.hpp"
#include "stdafx.h"

namespace MyPhysics {

using namespace std;

class MyForceGenerator {
    //* ╔════════════╗
    //* ║ Attributes ║
    //* ╚════════════╝
protected:

    //* ╔═══════════════════════════════╗
    //* ║ Constructors & Deconstructors ║
    //* ╚═══════════════════════════════╝
public:
    MyForceGenerator();

    //* ╔═════════╗
    //* ║ Methods ║
    //* ╚═════════╝
public:
    virtual void updateForce(MyParticle* targetParticle, float time);

private:
    //* ╔═══════════════════╗
    //* ║ Getters & Setters ║
    //* ╚═══════════════════╝
public:
};
}  // namespace MyPhysics