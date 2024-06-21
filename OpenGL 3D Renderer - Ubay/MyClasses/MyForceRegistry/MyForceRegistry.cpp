#include "MyForceRegistry.hpp"

using namespace MyPhysics;

//* ╔═══════════════════════════════╗
//* ║ Constructors & Deconstructors ║
//* ╚═══════════════════════════════╝
MyForceRegistry::MyForceRegistry() {}
//* ╔═════════╗
//* ║ Methods ║
//* ╚═════════╝
void MyForceRegistry::add(MyParticle* particle, MyForceGenerator* generator) {
    MyParticleForceRegistry newRegistry = {};
    newRegistry.particle                = particle;
    newRegistry.generator               = generator;
    for (MyParticleForceRegistry registry : this->registry) {
        //? Generators should only affect the same particle once.
        if (newRegistry.particle == registry.particle &&
            newRegistry.generator == registry.generator)
            return;
    }
    this->registry.push_back(newRegistry);
}
void MyForceRegistry::remove(MyParticle* particle, MyForceGenerator* generator) {
    this->registry.remove_if([particle, generator](MyParticleForceRegistry force) {
        return force.particle == particle && force.generator == generator;
    });
}
void MyForceRegistry::clear() { this->registry.clear(); }
void MyForceRegistry::updateForces(float time) {
    for (MyParticleForceRegistry force : this->registry) {
        force.generator->updateForce(force.particle, time);
    }
}
list<MyForceGenerator*> MyForceRegistry::getAppliedForces() {
    list<MyForceGenerator*> appliedForces;
    MyForceGenerator* lastAddedAppliedForceToList = this->registry.begin()->generator;
    for (MyParticleForceRegistry registeredForce : this->registry) {
        //? If the force we're looking at hasn't been added to the list, then add it.
        if (find(appliedForces.begin(), appliedForces.end(), registeredForce.generator) ==
            appliedForces.end()) {
            appliedForces.push_back(registeredForce.generator);
        }
    }
    return appliedForces;
}
list<MyParticle*> MyForceRegistry::getAffectedParticles() {
    list<MyParticle*> affectedParticles;
    MyParticle* lastAddedAffectedParticleToList = this->registry.begin()->particle;
    for (MyParticleForceRegistry registeredParticle : this->registry) {
        //? If the force we're looking at hasn't been added to the list, then add it.
        if (find(affectedParticles.begin(), affectedParticles.end(), registeredParticle.particle) ==
            affectedParticles.end()) {
            affectedParticles.push_back(registeredParticle.particle);
        }
    }
    return affectedParticles;
}

//* ╔═══════════════════╗
//* ║ Getters & Setters ║
//* ╚═══════════════════╝
list<MyParticleForceRegistry> MyForceRegistry::getRegistry() { return this->registry; }
