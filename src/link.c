#include "link.h"
#include "verlet.h"

void apply_link(struct Link link) {
    move_to_target(link.object_1, link.object_2, link.target_dist);
}
