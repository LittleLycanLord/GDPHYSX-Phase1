#include "MyParticle.hpp"

using namespace MyPhysics;

//* ╔═══════════════════════════════╗
//* ║ Constructors & Deconstructors ║
//* ╚═══════════════════════════════╝
MyParticle::MyParticle() : mass(1.0f), position(0.0f) {}
MyParticle::MyParticle(double mass) : mass(mass), position(0.0f) {}
MyParticle::MyParticle(MyVector3 position) : mass(1.0f), position(position) {}
MyParticle::MyParticle(double mass, MyVector3 position) : mass(mass), position(position) {}

//* ╔═════════╗
//* ║ Methods ║
//* ╚═════════╝
void MyParticle::updateModel() {}
void MyParticle::updatePosition(double time) {
    MyVector3 initialPosition(this->position);
    this->position =
        this->position + (this->velocity * time) + ((this->acceleration * time * time) / 2.0f);
    MyVector3 finalPosition(this->position);
    this->magnitudeVelocity = sqrt(pow((finalPosition.x - initialPosition.x), 2) +
                                   pow((finalPosition.y - initialPosition.y), 2) +
                                   pow((finalPosition.z - initialPosition.z), 2)) *
                              FRAMERATE;
    this->updateModel();
}
void MyParticle::updateVelocity(double time) {
    this->acceleration += this->accumulatedForce * (1 / this->mass);
    this->velocity = this->velocity + (this->acceleration * time);
    this->velocity = this->velocity * pow(damping, time);
}
void MyParticle::updateAverageVelocity(int physicsUpdateCount) {
    this->totalVelocity += this->velocity;
    this->averageVelocity = this->totalVelocity / physicsUpdateCount;
}

void MyParticle::update(double time, int physicsUpdateCount) {
    this->updatePosition(time);
    this->updateVelocity(time);
    //* - - - - - ADDITIONAL UPDATES - - - - -
    this->updateAverageVelocity(physicsUpdateCount);
    if (this->lifetime <= 0.0f) {
        this->destroy();
    } else {
        this->lifetime -= time;
    }
    //* - - - - - END OF ADDITIONAL UPDATES - - - - -
    this->resetForce();
}
void MyParticle::moveTowards(MyVector3 target, double magnitudeVelocity) {
    this->velocity = MyVector3(
        (magnitudeVelocity * (target.x - this->position.x)) /
            sqrt(pow((target.x - this->position.x), 2) + pow((target.y - this->position.y), 2) +
                 pow((target.z - this->position.z), 2)),
        (magnitudeVelocity * (target.y - this->position.y)) /
            sqrt(pow((target.x - this->position.x), 2) + pow((target.y - this->position.y), 2) +
                 pow((target.z - this->position.z), 2)),
        (magnitudeVelocity * (target.z - this->position.z)) /
            sqrt(pow((target.x - this->position.x), 2) + pow((target.y - this->position.y), 2) +
                 pow((target.z - this->position.z), 2)));
}
void MyParticle::addForce(MyVector3 force) { this->accumulatedForce += force; }
void MyParticle::resetForce() {
    this->accumulatedForce = MyVector3();
    this->acceleration     = MyVector3();
}
void MyParticle::extendLifetime(double amount) { this->lifetime += amount; }
void MyParticle::stop() {
    this->velocity     = MyVector3();
    this->acceleration = MyVector3();
}
void MyParticle::destroy() {
    this->isDestroyed = true;
    this->updateModel();
}

//* ╔═══════════════════╗
//* ║ Getters & Setters ║
//* ╚═══════════════════╝
My3DModel* MyParticle::getModel3D() { return NULL; }
double MyParticle::getMass() { return this->mass; }
void MyParticle::setMass(double mass) { this->mass = mass; }
double MyParticle::getDamping() { return this->damping; }
void MyParticle::setDamping(double damping) { this->damping = damping; }
double MyParticle::getLifetime() { return this->lifetime; }
void MyParticle::setLifetime(double lifetime) { this->lifetime = lifetime; }
bool MyParticle::getIsDestroyed() { return this->isDestroyed; }
bool MyParticle::getUsesGravity() { return this->usesGravity; }
void MyParticle::setUsesGravity(bool usesGravity) { this->usesGravity = usesGravity; }
MyVector3 MyParticle::getPosition() { return this->position; }
void MyParticle::setPosition(MyVector3 position) { this->position = position; }
void MyParticle::setPosition(double x, double y, double z) { this->position = MyVector3(x, y, z); }
double MyParticle::getMagnitudeVelocity() { return this->magnitudeVelocity; }
MyVector3 MyParticle::getAverageVelocity() { return this->averageVelocity; }
MyVector3 MyParticle::getVelocity() { return this->velocity; }
void MyParticle::setVelocity(MyVector3 velocity) { this->velocity = velocity; }
void MyParticle::setVelocity(double x, double y, double z) { this->velocity = MyVector3(x, y, z); }
MyVector3 MyParticle::getAcceleration() { return this->acceleration; }
void MyParticle::setAcceleration(MyVector3 acceleration) {
    this->acceleration = acceleration / FRAMERATE;
}
void MyParticle::setAcceleration(double x, double y, double z) {
    this->acceleration = MyVector3(x, y, z);
}