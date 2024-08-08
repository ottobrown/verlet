#include "stdlib.h"
#include "raylib.h"
#include "verlet.h"
#include "util.h"
#include "link.h"

/// The "world" all the physics takes place in,
/// containing objects, constraints, and constants.
struct World {
    /// Number of substeps per frame
    size_t substeps;

    size_t n_objects;
    size_t n_objects_allocated;
    struct VerletObject* objects;

    size_t n_links;
    size_t n_links_allocated;
    struct Link* links;

    /// Acceleration vector of gravitiational force
    Vector2 gravity;

    /// Center of containing circle
    Vector2 constraint_center;
    /// Radius of containing circle
    float constraint_radius;
};

/// Create a new `struct World` with no objects
struct World init_world();

void add_object(struct World* world, struct VerletObject obj);

void add_link(struct World* world, size_t idx1, size_t idx2, float target_dist);

void update_world(struct World* world, float dt);

/// `free` all pointers associated with `struct World`
void free_world(struct World *world);
