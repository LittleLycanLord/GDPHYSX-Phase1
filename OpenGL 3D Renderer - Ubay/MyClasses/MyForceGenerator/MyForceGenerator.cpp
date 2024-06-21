#include "MyForceGenerator.hpp"

using namespace MyPhysics;

//* ╔═══════════════════════════════╗
//* ║ Constructors & Deconstructors ║
//* ╚═══════════════════════════════╝
MyForceGenerator::MyForceGenerator() {}
//* ╔═════════╗
//* ║ Methods ║
//* ╚═════════╝
void MyForceGenerator::updateForce(MyParticle* targetParticle, float time) {
    targetParticle->addForce(MyVector3());
}

//* ╔═══════════════════╗
//* ║ Getters & Setters ║
//* ╚═══════════════════╝
