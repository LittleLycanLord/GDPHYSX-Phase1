#pragma once
#include "../../MyParticle/MyParticle.hpp"
#include "../../MyParticleContact/MyParticleContact.hpp"
#include "../../MyVectors/MyVector3.hpp"
#include "../MyParticleLink.hpp"
#include "stdafx.h"

namespace MyPhysics {

using namespace std;

class MyRod : public MyParticleLink {
    //* ╔════════════╗
    //* ║ Attributes ║
    //* ╚════════════╝
protected:
    double length;
    double restitution = 0.0f;

    //* ╔═══════════════════════════════╗
    //* ║ Constructors & Deconstructors ║
    //* ╚═══════════════════════════════╝
public:
    MyRod(MyParticle* particleA, MyParticle* particleB, double length);

    //* ╔═════════╗
    //* ║ Methods ║
    //* ╚═════════╝
public:
    virtual MyParticleContact* getContact() override;

protected:
private:
    //* ╔═══════════════════╗
    //* ║ Getters & Setters ║
    //* ╚═══════════════════╝
public:
};
}  // namespace MyPhysics