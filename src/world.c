#include "world.h"
#include "link.h"
#include "util.h"
#include "verlet.h"
#include <raylib.h>

struct World init_world() {
    struct World new = {
        .substeps = 8,

        .n_objects = 0,
        .n_objects_allocated = 5,
        .objects = calloc(5, sizeof(struct VerletObject)),
        .n_links = 0,
        .n_links_allocated = 5,
        .links = calloc(5, sizeof(struct Link)),

        .gravity = { 0.0, 1000.0 },
        .friction = 0.2,

        .constraint_center = { 600.0, 600.0 },
        .constraint_radius = 500.0,
    };

    return new;
}

void add_object(struct World *world, struct VerletObject obj) {
    if (world->n_objects >= world->n_objects_allocated) {
        world->n_objects_allocated = 2 * ( 1 + world->n_objects );
        struct VerletObject* old_p = world->objects;
        struct VerletObject* new_p = realloc(world->objects, world->n_objects_allocated * sizeof(struct VerletObject));
        world->objects = new_p;

        // When the `world->objects` array is moved, the pointers on the `Link`s become invalid.
        // So we translate them to the new location
        for(size_t l=0; l<world->n_links; l++) {
            world->links[l].object_1 = world->links[l].object_1 - old_p + new_p;
            world->links[l].object_2 = world->links[l].object_2 - old_p + new_p;
        }
    }

    world->objects[world->n_objects] = obj;
    world->n_objects ++;
}

void substep(struct World* world, float dt) {
    for (size_t i=0; i<world->n_objects; i++) {
        struct VerletObject* obj_ptr = &world->objects[i];
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
            struct VerletObject obj1 = world->objects[i], obj2 = world->objects[j];
            bool colliding = CheckCollisionCircles(
                obj1.current_pos,
                obj1.radius,
                obj2.current_pos,
                obj2.radius
            );

            if (colliding) {
                move_to_target(&world->objects[i], &world->objects[j], obj1.radius + obj2.radius);
            }
        }

        // update the position of each object
        verlet_update(obj_ptr, world->friction, dt);
    }

    for( size_t i=0; i<world->n_links; i++ ) {
        apply_link(world->links[i]);
    }
}

void add_link(struct World *world, size_t idx1, size_t idx2, float target_dist) {
    if (world->n_links >= world->n_links_allocated) {
        world->n_links_allocated = 2 * ( 1 + world->n_links );
        world->links= realloc(world->links, world->n_links_allocated * sizeof(struct Link));
    }

    struct Link link = {
        .object_1 = &world->objects[idx1],
        .object_2 = &world->objects[idx2],
        .target_dist = target_dist,
    };

    world->links[world->n_links] = link;
    world->n_links ++;
}


void update_world(struct World* world, float dt) {
    for(size_t step = 0; step < world->substeps; step++) {
        substep(world, dt/(float) world->substeps);
    }
}

void free_world(struct World *world) {
    free(world->objects);
    free(world->links);
}
