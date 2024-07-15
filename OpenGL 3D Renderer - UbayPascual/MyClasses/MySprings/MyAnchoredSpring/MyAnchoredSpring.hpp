#pragma once
#include "../../MyForceGenerator/MyForceGenerator.hpp"
#include "../../MyVectors/MyVector3.hpp"
#include "stdafx.h"

namespace MyPhysics {

using namespace std;

class MyAnchoredSpring : public MyForceGenerator {
    //* ╔════════════╗
    //* ║ Attributes ║
    //* ╚════════════╝
protected:
    MyVector3 anchorPoint;
    double springConstant;
    double restLength;
    //* ╔═══════════════════════════════╗
    //* ║ Constructors & Deconstructors ║
    //* ╚═══════════════════════════════╝
public:
    MyAnchoredSpring(MyVector3 anchorPoint, double springConstant, double restLength);

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