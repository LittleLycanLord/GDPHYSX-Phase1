#pragma once
#include "MyVector2.hpp"
#include "glad/glad.h"
#include "iomanip"
#include "stdafx.h"

namespace MyPhysics {
class MyVector3 {
    //* ╔════════════╗
    //* ║ Attributes ║
    //* ╚════════════╝
public:
    double x = 0;
    double y = 0;
    double z = 0;

    //* ╔═══════════════════════════════╗
    //* ║ Constructors & Deconstructors ║
    //* ╚═══════════════════════════════╝
public:
    MyVector3();
    MyVector3(double xyz);
    MyVector3(double x, double y, double z);
    MyVector3(glm::vec3 xyz);

    //* ╔═════════╗
    //* ║ Methods ║
    //* ╚═════════╝
public:
    void DisplayValuesIndividually(std::string message, int precision);
    void DisplayValues(int precision);

    //? Custom Typecast Override
    explicit operator glm::vec3() const { return glm::vec3(this->x, this->y, this->z); }
    explicit operator glm::vec2() const { return glm::vec2(this->x, this->y); };
    // explicit operator MyPhysics::MyVector2() const { return MyVector2(this->x, this->y); };

    //? Vector Operations
    MyVector3 operator+(const MyVector3 addend);
    void operator+=(const MyVector3 addend);

    MyVector3 operator-(const MyVector3 subtrahend);
    void operator-=(const MyVector3 subtrahend);

    MyVector3 operator*(const double multiplicand);
    void operator*=(const double multiplicand);
    MyVector3 operator*(const MyVector3 multiplicand);
    void operator*=(const MyVector3 multiplicand);
    double DotMultiplication(MyVector3 multiplicand);
    MyVector3 CrossMultiplication(MyVector3 multiplicand);

    MyVector3 operator/(const double dividend);
    void operator/=(const double dividend);
    MyVector3 operator/(const MyVector3 dividend);
    void operator/=(const MyVector3 dividend);
    double getMagnitude();
    MyVector3 getNormalized();
    //* ╔═══════════════════╗
    //* ║ Getters & Setters ║
    //* ╚═══════════════════╝
public:
    double getX();
    void setX(double x);
    double getY();
    void setY(double y);
    double getZ();
    void setZ(double z);
};
}  // namespace MyPhysics