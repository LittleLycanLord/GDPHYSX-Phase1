#pragma once
#include "../MyParticle/MyParticle.hpp"
#include "../MyParticleContact/MyParticleContact.hpp"
#include "../MyVectors/MyVector3.hpp"
#include "stdafx.h"

namespace MyPhysics {

using namespace std;

class MyParticleLink {
    //* ╔════════════╗
    //* ║ Attributes ║
    //* ╚════════════╝
protected:
    MyParticle* particleA;
    MyParticle* particleB;

    //* ╔═══════════════════════════════╗
    //* ║ Constructors & Deconstructors ║
    //* ╚═══════════════════════════════╝
public:
    MyParticleLink(MyParticle* particleA, MyParticle* particleB);

    //* ╔═════════╗
    //* ║ Methods ║
    //* ╚═════════╝
public:
    virtual MyParticleContact* getContact();

protected:
    double getDistanceBetweenParticles();

private:
    //* ╔═══════════════════╗
    //* ║ Getters & Setters ║
    //* ╚═══════════════════╝
public:
};
}  // namespace MyPhysics