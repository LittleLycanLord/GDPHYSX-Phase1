#pragma once
#include "../My3DModel/My3DModel.hpp"
#include "../MyContactResolver/MyContactResolver.hpp"
#include "../MyForceGenerator/MyGravityGenerator/MyGravityGenerator.hpp"
#include "../MyForceRegistry/MyForceRegistry.hpp"
#include "../MyParticle/MyParticle.hpp"
#include "../MyParticle/MyRenderParticle/MyRenderParticle.hpp"
#include "../MyParticleContact/MyParticleContact.hpp"
#include "../MyParticleLink/MyParticleLink.hpp"
#include "../MyParticleLink/MyRod/MyRod.hpp"
#include "../MySprings/MyAnchoredSpring/MyAnchoredSpring.hpp"
#include "../MySprings/MyParticleSpring/MyParticleSpring.hpp"
#include "stdafx.h"

using namespace std;
using namespace rendering;

namespace MyPhysics {
class MyPhysicsWorld {
    //* ╔════════════╗
    //* ║ Attributes ║
    //* ╚════════════╝
public:
    list<My3DModel*>* renderingList;
    list<MyParticle*> particles;
    list<MyParticleLink*> particleLinks;
    vector<MyParticleContact*> contacts;
    int updateCount;
    MyForceRegistry forceRegistry;
    MyGravityGenerator gravityGenerator;
    MyContactResolver contactResolver;

    //* ╔═══════════════════════════════╗
    //* ║ Constructors & Deconstructors ║
    //* ╚═══════════════════════════════╝
public:
    MyPhysicsWorld(list<My3DModel*>* renderingList);

    //* ╔═════════╗
    //* ║ Methods ║
    //* ╚═════════╝
public:
    void update(double time);
    void addParticle(MyParticle* particleToAdd);
    void addParticles(vector<MyParticle*> particleToAdd);
    void addParticleContact(MyParticle* particleA,
                            MyParticle* particleB,
                            double restitution,
                            double depth,
                            MyVector3 direction);
    void addSpring(MyParticle* particle,
                   MyVector3 anchorPoint,
                   double springConstant,
                   double restLength);
    void addSpring(MyParticle* particleA,
                   MyParticle* particleB,
                   double springConstant,
                   double restLength);
    void addRod(MyParticle* particleA, MyParticle* particleB, double restilengthtution);

protected:
    void generateContacts();
    void updateParticleList();
    void updateGravity(MyParticle* particle);
    void getOverlaps();

    //* ╔═══════════════════╗
    //* ║ Getters & Setters ║
    //* ╚═══════════════════╝
public:
    list<My3DModel*>* getRenderingList();
    vector<MyParticle*> getParticleListAsVector();
    MyForceRegistry* getForceRegistry();
};
}  // namespace MyPhysics