#pragma once
#include "MyVector3.hpp"
#include "glad/glad.h"
#include "iomanip"
#include "stdafx.h"

namespace MyPhysics {
class MyVector2 {
    //* ╔════════════╗
    //* ║ Attributes ║
    //* ╚════════════╝
protected:
    double x = 0;
    double y = 0;

    //* ╔═══════════════════════════════╗
    //* ║ Constructors & Deconstructors ║
    //* ╚═══════════════════════════════╝
public:
    MyVector2();
    MyVector2(double xy);
    MyVector2(double x, double y);
    MyVector2(glm::vec2 xy);

    //* ╔═════════╗
    //* ║ Methods ║
    //* ╚═════════╝
public:
    void DisplayValuesIndividually(std::string message, int precision);
    void DisplayValues(int precision);

    //? Custom Typecast Override
    explicit operator glm::vec2() const { return glm::vec2(this->x, this->y); }
    explicit operator glm::vec3() const { return glm::vec3(this->x, this->y, 0.0f); };
    // explicit operator MyPhysics::MyVector3() const { return MyVector3(this->x, this->y, 0.0f); };

    //? Vector Operations
    MyVector2 operator+(const MyVector2 addend);
    void operator+=(const MyVector2 addend);

    MyVector2 operator-(const MyVector2 subtrahend);
    void operator-=(const MyVector2 subtrahend);

    MyVector2 operator*(const double multiplicand);
    void operator*=(const double multiplicand);
    MyVector2 operator*(const MyVector2 multiplicand);
    void operator*=(const MyVector2 multiplicand);
    double DotMultiplication(MyVector2 multiplicand);

    MyVector2 operator/(const double dividend);
    void operator/=(const double dividend);
    MyVector2 operator/(const MyVector2 dividend);
    void operator/=(const MyVector2 dividend);
    double getMagnitude();
    MyVector2 getNormalized();
    //* ╔═══════════════════╗
    //* ║ Getters & Setters ║
    //* ╚═══════════════════╝
public:
    double getX();
    void setX(double x);
    double getY();
    void setY(double y);
};
}  // namespace MyPhysics