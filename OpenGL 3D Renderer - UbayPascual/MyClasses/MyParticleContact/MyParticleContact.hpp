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
    bool isRod = false;

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
    void resolveInterpenetrationWithoutConsiderationToMass(double time);
    void resolveInterpenetrationWithConsiderationToMass(double time);
    void resolveVelocityWithConsiderationToMass(double time);
    void resolveVelocityWithoutConsiderationToMass(double time);

public:
    double calculateSeparatingSpeed();
    void resolve(double time);

    //* ╔═══════════════════╗
    //* ║ Getters & Setters ║
    //* ╚═══════════════════╝
public:
    double getDepth();
    void setDepth(double depth);
    bool getIsRod();
    void setIsRod(bool isRod);
    double getRestitition();
    void setRestitition(double restitition);
    MyVector3 getContactNormal();
    void setContactNormal(MyVector3 contactNormal);
};
}  // namespace MyPhysics