#pragma once
#include "../MyParticleContact/MyParticleContact.hpp"
#include "../MyVectors/MyVector3.hpp"
#include "algorithm"
#include "stdafx.h"

using namespace rendering;
using namespace std;

namespace MyPhysics {
class MyContactResolver {
    //* ╔════════════╗
    //* ║ Attributes ║
    //* ╚════════════╝
private:
    unsigned int maxIterations;
    unsigned int currentIteration;

    //* ╔═══════════════════════════════╗
    //* ║ Constructors & Deconstructors ║
    //* ╚═══════════════════════════════╝
public:
    MyContactResolver(unsigned int maxIterations);

    //* ╔═════════╗
    //* ║ Methods ║
    //* ╚═════════╝
public:
    void resolveContacts(vector<MyParticleContact*> contacts, double time);

    //* ╔═══════════════════╗
    //* ║ Getters & Setters ║
    //* ╚═══════════════════╝
public:
};
}  // namespace MyPhysics