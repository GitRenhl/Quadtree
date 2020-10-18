#pragma once
#include "Vector2d.h"


class StarObject {
    static constexpr float m_GRAVITY_C = 0.000000006674f;
public:
    StarObject(float x, float y, uint32_t mass);
    ~StarObject() {}

    void AddForce(const Vector2f &force);
    Vector2f CalcForce(const StarObject &other) const;

    void Update();

private:
    void ApplayAcc();
private:
    Vector2f position;
    Vector2f velocity;
    Vector2f acceleration;
    /* A mass must be equal {obj_mass} / {earth_mass} so
       the smallest mass is {earth_mass} and in this variable is equal 1. */
    // {earth_mass} == 5.972 * (10^24)
    const uint32_t mass; 
};
