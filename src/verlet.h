#pragma once
#include "raylib.h"
#include "util.h"

struct VerletObject {
    Vector2 current_pos;
    Vector2 last_pos;
    Vector2 accel;

    /// A fixed object can't move
    bool fixed;

    // right now all objects are circles
    float radius;
};

/// Create a new `struct VerletObject` of the given position and radius,
/// With no velocity or acceleration and unfixed
struct VerletObject new_obj(float x, float y, float r);

void verlet_update(struct VerletObject* obj, float dt);

void accelerate(struct VerletObject *obj, Vector2 acc);

// Move two `struct VerletObject`s away from each other to be `target_dist` apart
void move_to_target(struct VerletObject* obj1, struct VerletObject* obj2, float target_dist);
