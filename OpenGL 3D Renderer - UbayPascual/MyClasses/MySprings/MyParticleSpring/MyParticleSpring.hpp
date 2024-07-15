#pragma once
#include "../../MyForceGenerator/MyForceGenerator.hpp"
#include "../../MyParticle/MyParticle.hpp"
#include "../../MyVectors/MyVector3.hpp"
#include "stdafx.h"

namespace MyPhysics {

using namespace std;

class MyParticleSpring : public MyForceGenerator {
    //* ╔════════════╗
    //* ║ Attributes ║
    //* ╚════════════╝
protected:
    MyParticle* otherParticle;
    double springConstant;
    double restLength;
    //* ╔═══════════════════════════════╗
    //* ║ Constructors & Deconstructors ║
    //* ╚═══════════════════════════════╝
public:
    MyParticleSpring(MyParticle* otherParticle, double springConstant, double restLength);

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