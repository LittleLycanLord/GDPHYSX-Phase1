#include "MyContactResolver.hpp"

using namespace MyPhysics;

//* ╔═══════════════════════════════╗
//* ║ Constructors & Deconstructors ║
//* ╚═══════════════════════════════╝
MyContactResolver::MyContactResolver(unsigned int maxIterations)
    : maxIterations(maxIterations),
      currentIteration(0) {}

//* ╔═════════╗
//* ║ Methods ║
//* ╚═════════╝
void MyContactResolver::resolveContacts(vector<MyParticleContact*> contacts, double time) {
    this->currentIteration = 0;
    while (this->currentIteration < this->maxIterations) {
        unsigned int currentIndexWithMinimumSpeed = 0;
        unsigned int currentIndexWithMaximumDepth = 0;
        unsigned int indexToResolve               = 0;
        double lowestSeparatingSpeed              = contacts[0]->calculateSeparatingSpeed();
        double maximumDepth                       = contacts[0]->getDepth();

        for (int i = 1; i < contacts.size(); i++) {
            double currentSeparatingSpeed = contacts[i]->calculateSeparatingSpeed();
            double currentDepth           = contacts[i]->getDepth();

            if (currentSeparatingSpeed < lowestSeparatingSpeed && currentDepth > 0) {
                currentIndexWithMinimumSpeed = i;
                lowestSeparatingSpeed        = currentSeparatingSpeed;
            } else {
                if (currentDepth > maximumDepth && currentSeparatingSpeed <= 0) {
                    currentIndexWithMaximumDepth = i;
                    maximumDepth                 = currentDepth;
                }
            }
        }
        if (lowestSeparatingSpeed >= 0 && maximumDepth <= 0) {
            return;
        }

        if (lowestSeparatingSpeed < 0 && maximumDepth <= 0)
            indexToResolve = currentIndexWithMinimumSpeed;
        else if (lowestSeparatingSpeed >= 0 && maximumDepth > 0)
            indexToResolve = currentIndexWithMaximumDepth;

        if (lowestSeparatingSpeed < 0 || maximumDepth > 0) {
            contacts[indexToResolve]->resolve(time);
            contacts.erase(contacts.begin() + indexToResolve);
        }
        if (contacts.empty()) return;
        this->currentIteration++;
    }

    // // ? Original Sir's Code
    // this->currentIteration = 0;

    // while (this->currentIteration < this->maxIterations) {
    //     unsigned current_index = 0;
    //     float curr_min         = contacts[0]->calculateSeparatingSpeed();
    //     float max_depth        = contacts[0]->getDepth();

    //     for (int i = 1; i < contacts.size(); i++) {
    //         float ss = contacts[i]->calculateSeparatingSpeed();

    //         if (ss < curr_min && ss < 0 || 0 < contacts[i]->getDepth()) {
    //             current_index = i;
    //             curr_min      = ss;

    //             if (max_depth < contacts[i]->getDepth()) max_depth = contacts[i]->getDepth();
    //         }
    //     }

    //     if (curr_min >= 0 && max_depth <= 0) return;

    //     contacts[current_index]->resolve(time);
    //     this->currentIteration++;
    // }
}

//* ╔═══════════════════╗
//* ║ Getters & Setters ║
//* ╚═══════════════════╝