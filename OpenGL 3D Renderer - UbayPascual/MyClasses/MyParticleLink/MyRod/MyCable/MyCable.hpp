#pragma once
#include "../../../MyParticle/MyParticle.hpp"
#include "../../../MyParticleContact/MyParticleContact.hpp"
#include "../../../MyVectors/MyVector3.hpp"
#include "../MyRod.hpp"
#include "stdafx.h"

namespace MyPhysics {

using namespace std;

class MyCable : public MyRod {
    //* ╔════════════╗
    //* ║ Attributes ║
    //* ╚════════════╝
protected:

    //* ╔═══════════════════════════════╗
    //* ║ Constructors & Deconstructors ║
    //* ╚═══════════════════════════════╝
public:
    MyCable(MyParticle* particleA, MyParticle* particleB, double length);

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