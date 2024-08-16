#include "world.h"
#include "dynarray.h"
#include "link.h"
#include "util.h"
#include "verlet.h"
#include <raylib.h>

struct World init_world() {
    struct World new = {
        .substeps = 8,

        .gravity = { 0.0, 1000.0 },
        .friction = 0.2,

        .constraint_center = { 600.0, 600.0 },
        .constraint_radius = 500.0,
    };

    da_init(new.objects, struct VerletObject);
    da_init(new.links, struct Link);

    return new;
}

void add_object(struct World *world, struct VerletObject obj) {
    struct VerletObject* old_p = world->objects.data;
    da_add(world->objects, obj);
    struct VerletObject* new_p = world->objects.data;

    if (old_p != new_p) {
        // When the `world->objects` array is moved, the pointers on the `Link`s become invalid.
        // So we translate them to the new location
        for(size_t l=0; l<da_len(world->links); l++) {
            world->links.data[l].object_1 = world->links.data[l].object_1 - old_p + new_p;
            world->links.data[l].object_2 = world->links.data[l].object_2 - old_p + new_p;
        }
    }
}

void substep(struct World* world, float dt) {
    for (size_t i=0; i<da_len(world->objects); i++) {
        struct VerletObject* obj_ptr = &world->objects.data[i];
        // apply force of gravity to each object
        accelerate(obj_ptr, world->gravity);

        // apply constraint
        Vector2 to_obj = v2sub(obj_ptr->current_pos, world->constraint_center);
        float dist = v2len(to_obj);
        if (dist > world->constraint_radius - obj_ptr->radius) {
            Vector2 change = v2scl(v2nrm(to_obj), (world->constraint_radius - obj_ptr->radius));
            obj_ptr->current_pos = v2add(world->constraint_center, change);
        }

        // resolve collisions
        for(size_t j=0; j<i; j++) {
            struct VerletObject obj1 = world->objects.data[i], obj2 = world->objects.data[j];
            bool colliding = CheckCollisionCircles(
                obj1.current_pos,
                obj1.radius,
                obj2.current_pos,
                obj2.radius
            );

            if (colliding) {
                move_to_target(&world->objects.data[i], &world->objects.data[j], obj1.radius + obj2.radius);
            }
        }

        // update the position of each object
        verlet_update(obj_ptr, world->friction, dt);
    }

    for( size_t i=0; i<da_len(world->links); i++ ) {
        apply_link(world->links.data[i]);
    }
}

void add_link(struct World *world, size_t idx1, size_t idx2, float target_dist) {
    struct Link link = {
        .object_1 = &world->objects.data[idx1],
        .object_2 = &world->objects.data[idx2],
        .target_dist = target_dist,
    };
    (void) link;
    da_add(world->links, link);
}

void update_world(struct World* world, float dt) {
    for(size_t step = 0; step < world->substeps; step++) {
        substep(world, dt/(float) world->substeps);
    }
}

void free_world(struct World *world) {
    da_free(world->objects);
    da_free(world->links);
}
