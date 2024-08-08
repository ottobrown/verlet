#pragma once
#include "verlet.h"

/// A link between two `struct VerletObject`s that attempts to keep them at the
/// given distance apart
struct Link {
    struct VerletObject* object_1;
    struct VerletObject* object_2;
    float target_dist;
};

void apply_link(struct Link link);
