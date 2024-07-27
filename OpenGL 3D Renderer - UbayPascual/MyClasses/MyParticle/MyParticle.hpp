#pragma once
#include "../My3DModel/My3DModel.hpp"
#include "../MyNormal/MyNormal.hpp"
#include "../MyVectors/MyVector3.hpp"
#include "stdafx.h"

using namespace rendering;
using namespace std;

namespace MyPhysics {
class MyParticle {
    //* ╔════════════╗
    //* ║ Attributes ║
    //* ╚════════════╝
protected:
    double mass;                 //? in kg
    double radius;               //? in m
    double restitution;          //? in sticky units
    MyVector3 position;          //? in m
    MyVector3 originalPosition;  //? in m

    double damping    = DEFAULT_DAMPING;  //? [0 -> 1]; the lower the value, the higher the drag
    double lifetime   = 9999999.0f;       //? in s
    bool isDestroyed  = false;
    bool usesGravity  = false;
    bool hasCollision = true;
    bool lockPosition = false;
    double magnitudeVelocity   = 0.0f;         //? in m/s
    MyVector3 totalVelocity    = MyVector3();  //? in m/s
    MyVector3 averageVelocity  = MyVector3();  //? in m/s
    MyVector3 velocity         = MyVector3();  //? in m/s
    MyVector3 acceleration     = MyVector3();  //? in m/s^2

    MyVector3 accumulatedForce = MyVector3();  //? in kg m/s^2

    //* ╔═══════════════════════════════╗
    //* ║ Constructors & Deconstructors ║
    //* ╚═══════════════════════════════╝
public:
    MyParticle();
    MyParticle(vector<double> attributes);
    MyParticle(MyVector3 position);
    MyParticle(vector<double> attributes, MyVector3 position);

    //* ╔═════════╗
    //* ║ Methods ║
    //* ╚═════════╝
protected:
    virtual void updateModel();
    void updatePosition(double time);
    void updateVelocity(double time);
    void updateAverageVelocity(int physicsUpdateCount);

public:
    virtual void update(double time, int physicsUpdateCount);
    void moveTowards(MyVector3 target, double magnitudeVelocity);
    void addForce(MyVector3 force);
    void resetForce();
    void extendLifetime(double amount);
    void stop();
    void destroy();

    //* ╔═══════════════════╗
    //* ║ Getters & Setters ║
    //* ╚═══════════════════╝
public:
    virtual My3DModel* getModel3D();
    double getMass();
    void setMass(double mass);
    double getRadius();
    void setRadius(double radius);
    double getRestitution();
    void setRestitution(double restitution);
    double getDamping();
    void setDamping(double damping);
    double getLifetime();
    void setLifetime(double lifetime);
    bool getIsDestroyed();
    bool getUsesGravity();
    void setUsesGravity(bool usesGravity);
    bool getHasCollision();
    void setHasCollision(bool hasCollision);
    bool getLockPosition();
    void setLockPosition(bool lockPosition);
    MyVector3 getPosition();
    void setPosition(MyVector3 position);
    MyVector3 getOriginalPosition();
    void setOriginalPosition(MyVector3 originalPosition, bool updateCurrentPosition);
    void setPosition(double x, double y, double z);
    double getMagnitudeVelocity();
    MyVector3 getAverageVelocity();
    MyVector3 getVelocity();
    void setVelocity(MyVector3 velocity);
    void setVelocity(double x, double y, double z);
    MyVector3 getAcceleration();
    void setAcceleration(MyVector3 acceleration);
    void setAcceleration(double x, double y, double z);
};
}  // namespace MyPhysics