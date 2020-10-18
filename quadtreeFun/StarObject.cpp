#include "StarObject.h"


StarObject::StarObject(float x, float y, uint32_t mass):
    position(x, y),
    mass(mass){
    throw; // Not implemented
}

void StarObject::AddForce(const Vector2f &force) {
    acceleration += force;
}

Vector2f StarObject::CalcForce(const StarObject &other) const {
    Vector2f distance = position.DistanceTo(other.position);
    float force = m_GRAVITY_C * (mass * other.mass) / (distance * distance);
    Vector2f vForce = (position - other.position).Normalize() * force;
    return force;
}

void StarObject::Update() {
    ApplayAcc();
    position += velocity;
}

void StarObject::ApplayAcc() {
    velocity += acceleration;
    acceleration *= 0;
}
