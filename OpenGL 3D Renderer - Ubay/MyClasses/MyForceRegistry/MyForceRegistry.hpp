#pragma once
#include "../MyForceGenerator/MyForceGenerator.hpp"
#include "../MyParticle/MyParticle.hpp"
#include "stdafx.h"

namespace MyPhysics {

using namespace std;

struct MyParticleForceRegistry {
    MyForceGenerator* generator;
    MyParticle* particle;
};

class MyForceRegistry {
    //* ╔════════════╗
    //* ║ Attributes ║
    //* ╚════════════╝
protected:
    list<MyParticleForceRegistry> registry;

    //* ╔═══════════════════════════════╗
    //* ║ Constructors & Deconstructors ║
    //* ╚═══════════════════════════════╝
public:
    MyForceRegistry();

    //* ╔═════════╗
    //* ║ Methods ║
    //* ╚═════════╝
public:
    void add(MyParticle* particle, MyForceGenerator* generator);
    void remove(MyParticle* particle, MyForceGenerator* generator);
    void clear();
    void updateForces(float time);
    list<MyForceGenerator*> getAppliedForces();
    list<MyParticle*> getAffectedParticles();

private:
    //* ╔═══════════════════╗
    //* ║ Getters & Setters ║
    //* ╚═══════════════════╝
public:
    list<MyParticleForceRegistry> getRegistry();
};
}  // namespace MyPhysics