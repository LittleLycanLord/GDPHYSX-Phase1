#pragma once
#include "../../My3DModel/My3DModel.hpp"
#include "../../MyParticle/MyRenderParticle/MyRenderParticle.hpp"
#include "../../MyParticleLink/MyRod/MyRod.hpp"
#include "../../MyVectors/MyVector3.hpp"
#include "stdafx.h"

namespace MyPhysics {

using namespace std;
using namespace rendering;

class MyAnchoredChain {
    //* ╔════════════╗
    //* ║ Attributes ║
    //* ╚════════════╝
protected:
    My3DModel* model;
    MyVector3 tint;
    MyParticle* anchorParticle;
    unsigned int segmentCount;
    double segmentLength;
    double totalLength;
    bool usesGravity = true;

    vector<MyParticle*> particles;
    vector<MyRod*> segments;
    //* ╔═══════════════════════════════╗
    //* ║ Constructors & Deconstructors ║
    //* ╚═══════════════════════════════╝
public:
    //? Unknown chain segment count
    MyAnchoredChain(My3DModel* model,
                    MyVector3 tint,
                    MyParticle* anchorParticle,
                    double segmentLength,
                    double totalLength,
                    bool usesGravity);

    //? Unknown chain segment length
    MyAnchoredChain(My3DModel* model,
                    MyVector3 tint,
                    MyParticle* anchorParticle,
                    unsigned int segmentCount,
                    double totalLength,
                    bool usesGravity);
    //* ╔═════════╗
    //* ║ Methods ║
    //* ╚═════════╝
public:
    void addParticle(MyParticle* particle);
    void addSegment(MyRod* rod);
    void setHeadSegment(MyRod* headRod);

private:
    //* ╔═══════════════════╗
    //* ║ Getters & Setters ║
    //* ╚═══════════════════╝
public:
    My3DModel* getModel();
    MyVector3 getTint();
    MyParticle* getAnchorParticle();
    unsigned int getSegmentCount();
    double getSegmentLength();
    double getTotalLength();
    bool getUsesGravity();

    MyRod* getHeadSegment();
    vector<MyParticle*> getParticles();
    vector<MyRod*> getSegments();
};
}  // namespace MyPhysics