#pragma once
#include "../My3DModel/My3DModel.hpp"
#include "../MyForceGenerator/MyGravityGenerator/MyGravityGenerator.hpp"
#include "../MyForceRegistry/MyForceRegistry.hpp"
#include "../MyParticle/MyParticle.hpp"
#include "../MyParticle/MyRenderParticle/MyRenderParticle.hpp"
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
    int updateCount;
    MyForceRegistry forceRegistry;
    MyGravityGenerator gravityGenerator;

    //* ╔═══════════════════════════════╗
    //* ║ Constructors & Deconstructors ║
    //* ╚═══════════════════════════════╝
public:
    MyPhysicsWorld(list<My3DModel*>* renderingList);

    //* ╔═════════╗
    //* ║ Methods ║
    //* ╚═════════╝
public:
    void update(float time);
    void addParticle(MyParticle* particleToAdd);

private:
    void updateParticleList();
    void updateGravity(MyParticle* particle);

    //* ╔═══════════════════╗
    //* ║ Getters & Setters ║
    //* ╚═══════════════════╝
public:
    list<My3DModel*>* getRenderingList();
    vector<MyParticle*> getParticleListAsVector();
    MyForceRegistry* getForceRegistry();
};
}  // namespace MyPhysics