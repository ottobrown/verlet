#include "raylib.h"
#include "verlet.h"
#include "world.h"
#include <time.h>
#include <stdlib.h>

const Vector2 gravity = { 0.0f, 1000.0f };

struct Window {
    int width;
    int height;
    int fps;
    char* name;
};

int main() {
    struct Window window = {
        .width = 1200,
        .height = 1200,
        .fps = 60,
        .name = "2D Physics Sim",
    };

    InitWindow(window.width, window.height, window.name);
    SetTargetFPS(window.fps);

    srand(time(NULL));
    unsigned long frame = 0;

    struct World world = init_world();

    // initial link chain
    struct VerletObject left = new_obj(300.0, 600.0, 20.0);
    left.fixed = true;
    add_object(&world, left);
    struct VerletObject right = new_obj(900.0, 600.0, 20.0);
    right.fixed = true;
    add_object(&world, right);

    // fill in chain
    for(size_t c=1; c<=19; c++) {
        struct VerletObject mid = new_obj(900.0 - 30.0*c, 600.0, 15.0);
        add_object(&world, mid);
        add_link(&world, c, c+1, 35.0);
    };
    add_link(&world, 0, 20, 35.0);

    while(!WindowShouldClose()) {
        frame ++;
        update_world(&world, 1/60.0);

        // Click to add new ball
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            struct VerletObject spawn = new_obj(GetMouseX(), GetMouseY(), (float) (rand() % 10 + 20));
            add_object(&world, spawn);
        }

        BeginDrawing();

        ClearBackground(BLACK);

        // draw constraint circle
        DrawCircle(world.constraint_center.x, world.constraint_center.y, world.constraint_radius, LIGHTGRAY);
        // draw links
        for(size_t i=0; i<world.n_links; i++) {
            struct Link l = world.links[i];
            DrawLineEx(
                l.object_1->current_pos,
                l.object_2->current_pos,
                5.0,
                BLUE
            );
        }
        // draw objects
        for (size_t i=0; i<world.n_objects; i++) {
            struct VerletObject obj = world.objects[i];
            DrawCircle(obj.current_pos.x, obj.current_pos.y, obj.radius, RED);
        }

        EndDrawing();
    }

    free_world(&world);

    CloseWindow();

    return 0;
}
