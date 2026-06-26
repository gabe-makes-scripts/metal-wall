#include <cstdio>
#include <raylib.h>
#include <iostream>
#include <vector>
#include <fstream>
#include <raymath.h>

// run command  g++ main.cpp -lraylib -lGL -lm -lpthread -ldl -lrt -lX11 -o game
// last commit: previewing, snapping
// current goal: add snapping to lines

// config
float circle_size = 5;
float snap_distance = 15;

// varibles
bool creating_wall = false;
bool compiled = false;

Vector2 start_position = {-1.0f, -1.0f};
Vector2 end_position = {-1.0f, -1.0f};

// structs
struct wall {
    Vector2 start_pos;
    Vector2 end_pos;
};

// backend
int save(std::vector<wall>& walls) {
    std::ofstream outFile("build.txt");

    if (!outFile) {return 1;}

    for (const auto& wall : walls) {
        outFile << wall.start_pos.x << " " << wall.start_pos.y << " - " << wall.end_pos.x << ' ' << wall.end_pos.y << '\n';
    }

    outFile.close();
    compiled = true;
    return 0;
}

Vector2 snap(std::vector<wall>& walls, Vector2 current_position) {
    for (const auto& wall : walls) {
        if (Vector2Distance(current_position, wall.start_pos) <= snap_distance) {
            return wall.start_pos;
        } else if (Vector2Distance(current_position, wall.end_pos) <= snap_distance) {
            return wall.end_pos;
        }
    }

    return current_position;
}

// input
bool createWall(std::vector<wall>& walls) {
    if (creating_wall == false) {
        creating_wall = true;
        start_position = snap(walls, GetMousePosition());
    } else {
        creating_wall = false;
        end_position = snap(walls, GetMousePosition());
    }

    if (creating_wall == false) {
        std::cout << "wall complete" << '\n';
        walls.push_back({start_position, end_position});

        start_position = {-1.0f, -1.0f};
        end_position = {-1.0f, -1.0f};

        return true; 
    }

    return false;
}

// visual
void render(std::vector<wall>& walls) {
    for (const auto& wall : walls) {
        DrawCircle(wall.start_pos.x, wall.start_pos.y, circle_size, WHITE);
        DrawCircle(wall.end_pos.x, wall.end_pos.y, circle_size, WHITE);

        DrawLine(wall.start_pos.x, wall.start_pos.y, wall.end_pos.x, wall.end_pos.y, RAYWHITE);
    }
}

void preview(std::vector<wall>& walls) {
    if (start_position.x > -1.0f and start_position.y > -1.0f) {
        Vector2 endPos = snap(walls, GetMousePosition());
        DrawLine(start_position.x, start_position.y, endPos.x, endPos.y, GRAY);
    }
}

// main

int main () {

    InitWindow(800, 800, "bighard");
    SetTargetFPS(60);

    std::vector<wall> walls;

    while (!WindowShouldClose() and !compiled) {
        
        // input detection
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {std::cout << createWall(walls);}
        if (IsKeyPressed(KEY_ENTER)) {save(walls);}
        
        BeginDrawing();
            ClearBackground(BLACK);
            render(walls);
            preview(walls);
        EndDrawing();
    }

    return 0;
}
