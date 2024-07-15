#pragma once
#include "../MyParticle/MyParticle.hpp"
#include "../MyVectors/MyVector3.hpp"
#include "stdafx.h"

using namespace rendering;
using namespace std;

namespace MyPhysics {
class MyParticleContact {
    //* ╔════════════╗
    //* ║ Attributes ║
    //* ╚════════════╝
private:
    MyParticle* particleA;
    MyParticle* particleB;
    double restitition;
    double depth;
    MyVector3 contactNormal;
    
    //* ╔═══════════════════════════════╗
    //* ║ Constructors & Deconstructors ║
    //* ╚═══════════════════════════════╝
public:
    MyParticleContact(MyParticle* particleA,
                      MyParticle* particleB,
                      double restitition,
                      double depth,
                      MyVector3 contactNormal);

    //* ╔═════════╗
    //* ║ Methods ║
    //* ╚═════════╝
protected:
    void resolveInterpenetration(double time);
    void resolveVelocity(double time);

public:
    double calculateSeparatingSpeed();
    void resolve(double time);

    //* ╔═══════════════════╗
    //* ║ Getters & Setters ║
    //* ╚═══════════════════╝
public:
    double getDepth();
    void setDepth(double depth);
    double getRestitition();
    void setRestitition(double restitition);
    MyVector3 getContactNormal();
    void setContactNormal(MyVector3 contactNormal);
};
}  // namespace MyPhysics