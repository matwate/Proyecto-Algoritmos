#include <raylib.h>
#include "constants.h"
#include "Visualizer.h"
#include <iostream>

int main()
{
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Sorting Algorithms visualizer");
    Visualizer visualizer = Visualizer();
    SetTargetFPS(60);
    while (!WindowShouldClose()){
        
        visualizer.Update();
    
        BeginDrawing();
        ClearBackground(BLACK);
        DrawFPS(10,10);
        visualizer.Draw();
        

        DrawText("Press R to reset", 10, 130, 20, WHITE);
        DrawText("Press S to shuffle", 10, 150, 20, WHITE);
        DrawText("Press B to bubble sort", 10, 170, 20, WHITE);
        DrawText("Press L to selection sort", 10, 190, 20, WHITE);
        DrawText("Press I to insertion sort", 10, 210, 20, WHITE);
        DrawText("Press Q to quick sort", 10, 230, 20, WHITE);
        DrawText("Press M to merge sort", 10, 250, 20, WHITE);
        DrawText("Press H to heap sort", 10, 270, 20, WHITE);
        DrawText("Press D to Radix LSD sort", 10, 290, 20, WHITE);

        EndDrawing();



    }

    CloseWindow(); /*Remember this goes last */
}