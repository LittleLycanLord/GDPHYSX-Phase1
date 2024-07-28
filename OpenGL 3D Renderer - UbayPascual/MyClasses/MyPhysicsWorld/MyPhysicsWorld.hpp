#pragma once
#include "../My3DModel/My3DModel.hpp"
#include "../MyContactResolver/MyContactResolver.hpp"
#include "../MyForceGenerator/MyGravityGenerator/MyGravityGenerator.hpp"
#include "../MyForceRegistry/MyForceRegistry.hpp"
#include "../MyParticle/MyParticle.hpp"
#include "../MyParticle/MyRenderParticle/MyRenderParticle.hpp"
#include "../MyParticleConnections/MyAnchoredChain/MyAnchoredChain.hpp"
#include "../MyParticleConnections/MyAnchoredRope/MyAnchoredRope.hpp"
#include "../MyParticleContact/MyParticleContact.hpp"
#include "../MyParticleLink/MyParticleLink.hpp"
#include "../MyParticleLink/MyRod/MyCable/MyCable.hpp"
#include "../MyParticleLink/MyRod/MyRod.hpp"
#include "../MyRenderLine/MyParticleLine/MyParticleLine.hpp"
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
    list<MyParticleLine*> lines;
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
    void addParticle(MyParticle* particleToAdd, bool usesGravity);
    void addParticles(vector<MyParticle*> particleToAdd, bool usesGravity);
    void addParticleContact(MyParticle* particleA,
                            MyParticle* particleB,
                            double restitution,
                            double depth,
                            MyVector3 direction);
    MyAnchoredSpring* addSpring(MyParticle* particle,
                                MyVector3 anchorPoint,
                                double springConstant,
                                double restLength);
    vector<MyParticleSpring*> addSpring(MyParticle* particleA,
                                        MyParticle* particleB,
                                        double springConstant,
                                        double restLength);
    MyRod* addRod(MyParticle* particleA, MyParticle* particleB, double restilengthtution);
    MyCable* addCable(MyParticle* particleA, MyParticle* particleB, double restilengthtution);
    void addAnchoredRope(MyAnchoredRope* rope, bool showSegments);
    void addAnchoredChain(MyAnchoredChain* chain, bool showSegments);
    void setGravity(double newGravity);

protected:
    void generateContacts();
    void updateParticleList();
    void updateGravity(MyParticle* particle);
    void getOverlaps();
    void addLine(MyParticle* particleA, MyParticle* particleB);
    //* ╔═══════════════════╗
    //* ║ Getters & Setters ║
    //* ╚═══════════════════╝
public:
    list<My3DModel*>* getRenderingList();
    vector<MyParticle*> getParticleListAsVector();
    MyForceRegistry* getForceRegistry();
};
}  // namespace MyPhysics