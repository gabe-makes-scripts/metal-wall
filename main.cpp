#include <cstdio>
#include <raylib.h>
#include <iostream>
#include <vector>
#include <fstream>

// run command  g++ main.cpp -lraylib -lGL -lm -lpthread -ldl -lrt -lX11 -o game
// current goal: actually render walls

// config
float circle_size = 5;

// varibles
bool creating_wall = false;
bool compiled = false;

Vector2 start_position;
Vector2 end_position;

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
        outFile << wall.start_pos.x << " " << wall.start_pos.y << wall.end_pos.y << wall.end_pos.x << '\n';
    }

    outFile.close();
    compiled = true;
    return 0;
}

// input
bool createWall(std::vector<wall>& walls) {
    if (creating_wall == false) {
        creating_wall = true;
        start_position = GetMousePosition();
    } else {
        creating_wall = false;
        end_position = GetMousePosition();
    }

    if (creating_wall == false) {
        std::cout << "wall complete" << '\n';
        walls.push_back({start_position, end_position});

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
        EndDrawing();
    }

    return 0;
}
