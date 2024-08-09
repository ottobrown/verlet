#include "verlet.h"

struct VerletObject new_obj(float x, float y, float r) {
    struct VerletObject new = {
        .current_pos = { x, y },
        .last_pos = { x, y },
        .accel = V2NULL,
        .fixed = false,

        .radius = r,
    };

    return new;
}

void verlet_update(struct VerletObject* obj, float friction, float dt) {
    if (obj->fixed)
        return;

    float time_friction = friction * dt;
    // Verlet integration
    // x_{n+1} = 2x_n - x_{n-1} + A{\DELTA t}^2
    Vector2 new_pos = v2add(
        v2scl(obj->accel, dt*dt),
        v2sub(v2scl(obj->current_pos, 2.0 - time_friction), v2scl(obj->last_pos, 1.0 - time_friction))
    );

    obj->last_pos = obj->current_pos;
    obj->current_pos = new_pos;
    obj->accel = V2NULL;
}

void accelerate(struct VerletObject *obj, Vector2 acc) {
    obj->accel = v2add(obj->accel, acc);
}

void move_to_target(struct VerletObject* obj1, struct VerletObject* obj2, float target_dist) {
    Vector2 axis = v2sub(obj1->current_pos, obj2->current_pos);
    Vector2 norm = v2nrm(axis);
    float dist = v2len(axis);
    float delta = target_dist - dist;

    if (obj1->fixed && obj2->fixed) {
        return;
    }

    if ( !(obj1->fixed || obj2->fixed) ) {
        Vector2 change_each = v2scl(norm, 0.5f * delta);
        obj1->current_pos = v2add(obj1->current_pos, change_each);
        obj2->current_pos = v2sub(obj2->current_pos, change_each);
    }

    if (obj1->fixed) {
        obj2->current_pos = v2sub(obj2->current_pos, v2scl(norm, delta));
    }

    if (obj2->fixed) {
        obj1->current_pos = v2add(obj1->current_pos, v2scl(norm, delta));
    }
}
