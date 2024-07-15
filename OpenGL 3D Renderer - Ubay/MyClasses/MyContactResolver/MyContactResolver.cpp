#include "MyContactResolver.hpp"

using namespace MyPhysics;

//* ╔═══════════════════════════════╗
//* ║ Constructors & Deconstructors ║
//* ╚═══════════════════════════════╝
MyContactResolver::MyContactResolver(unsigned int maxIterations)
    : maxIterations(maxIterations),
      currentIterations(0) {}

//* ╔═════════╗
//* ║ Methods ║
//* ╚═════════╝
void MyContactResolver::resolveContacts(vector<MyParticleContact*> contacts, double time) {
    int currentIterations = 0;

    while (currentIterations < this->maxIterations) {
        unsigned int currentIndex    = 0;
        double lowestSeparatingSpeed = contacts[0]->calculateSeparatingSpeed();
        double maximumDepth          = contacts[0]->getDepth();

        for (int i = 1; i < contacts.size(); i++) {
            double separatingSpeed = contacts[i]->calculateSeparatingSpeed();

            if (separatingSpeed < lowestSeparatingSpeed &&
                (separatingSpeed < 0 || contacts[i]->getDepth() > 0)) {
                currentIndex          = i;
                lowestSeparatingSpeed = separatingSpeed;

                if (maximumDepth < contacts[i]->getDepth()) {
                    maximumDepth = contacts[i]->getDepth();
                }
            }
        }

        if (lowestSeparatingSpeed >= 0 && maximumDepth <= 0) {
            return;
        }

        contacts[currentIndex]->resolve(time);
        currentIterations++;
    }
    return;
}

//* ╔═══════════════════╗
//* ║ Getters & Setters ║
//* ╚═══════════════════╝