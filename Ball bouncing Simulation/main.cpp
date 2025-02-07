#include "raylib.h"
#include "raymath.h"
#include <iostream>
#include <cmath>

const int screenWidth = 800;
const int screenHeight = 800;
const float squareSize = 400.0f;
const float thickness = 10.0f;
const float rotationSpeed = 0.5f;
const float ballGrowth = 1.04f;
const float speedIncrease = 1.05f;

struct Ball {
    Vector2 position;
    Vector2 velocity;
    float radius;
};

Vector2 Reflect(Vector2 velocity, Vector2 normal) {
    float dotProduct = velocity.x * normal.x + velocity.y * normal.y;
    return {
        velocity.x - 2.0f * dotProduct * normal.x,
        velocity.y - 2.0f * dotProduct * normal.y
    };
}



bool CheckCollisionWithSquare(Ball &ball, float angle) {
    float halfSize = squareSize / 2.0f;
    float sinA = sin(angle * DEG2RAD);
    float cosA = cos(angle * DEG2RAD);
    
    Vector2 localPos = {
        cosA * (ball.position.x - (float)screenWidth / 2.0f) + sinA * (ball.position.y - (float)screenHeight / 2.0f),
        -sinA * (ball.position.x - (float)screenWidth / 2.0f) + cosA * (ball.position.y - (float)screenHeight / 2.0f)
    };
    
    float innerSize = halfSize - thickness - ball.radius;
    Vector2 normal = {0.0f, 0.0f};
    bool collision = false;
    
    if (fabs(localPos.x) > innerSize && fabs(localPos.x) < halfSize && fabs(localPos.y) < halfSize) {
        normal = {localPos.x > 0 ? 1.0f : -1.0f, 0.0f};
        collision = true;
    }
    if (fabs(localPos.y) > innerSize && fabs(localPos.y) < halfSize && fabs(localPos.x) < halfSize) {
        normal = {0.0f, localPos.y > 0 ? 1.0f : -1.0f};
        collision = true;
    }
    
    if (collision) {
        Vector2 worldNormal = {
            cosA * normal.x - sinA * normal.y,
            sinA * normal.x + cosA * normal.y
        };
        ball.velocity = Reflect(ball.velocity, worldNormal);
        ball.velocity.x *= speedIncrease;
        ball.velocity.y *= speedIncrease;
        ball.radius *= ballGrowth;
        
        localPos.x = Clamp(localPos.x, -innerSize, innerSize);
        localPos.y = Clamp(localPos.y, -innerSize, innerSize);

        std::cout << "Collision\n";

        ball.position = {
            (float)screenWidth / 2.0f + cosA * localPos.x - sinA * localPos.y,
            (float)screenHeight / 2.0f + sinA * localPos.x + cosA * localPos.y
        };
    }
    return collision;
}

int main() {
    InitWindow(screenWidth, screenHeight, "Rotating Hollow Square Simulation");
    SetTargetFPS(60);

    Ball ball = {{(float)screenWidth / 2.0f, (float)screenHeight / 2.0f}, {3.0f, 2.0f}, 10.0f};
    float rotation = 0.0f;
    
    while (!WindowShouldClose()) {
        rotation += rotationSpeed;
        ball.position.x += ball.velocity.x;
        ball.position.y += ball.velocity.y;
        
        CheckCollisionWithSquare(ball, rotation);
        
        BeginDrawing();
        ClearBackground(BLACK);
        
        DrawRectanglePro(
            {(float)screenWidth / 2.0f, (float)screenHeight / 2.0f, squareSize, squareSize},
            {squareSize / 2.0f, squareSize / 2.0f},
            rotation,
            BLUE
        );
        
        DrawRectanglePro(
            {(float)screenWidth / 2.0f, (float)screenHeight / 2.0f, squareSize - 2.0f * thickness, squareSize - 2.0f * thickness},
            {squareSize / 2.0f - thickness, squareSize / 2.0f - thickness},
            rotation,
            BLACK
        );
        
        DrawCircleV(ball.position, ball.radius, RED);
        
        EndDrawing();
    }
    CloseWindow();
    return 0;
}