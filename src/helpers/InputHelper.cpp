#include "InputHelper.hpp"

#include <raylib.h>

#include <string>

std::string Prompt(const std::string& message) {
    std::string inputText;
    bool isEnteringText = true;

    // Clear char pressed queue when entering the prompt
    GetCharPressed();

    while (isEnteringText && !WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(BLACK);

        // Capture user input
        int key = GetCharPressed();
        while (key > 0) {
            if ((key >= 32) && (key <= 125)) {
                inputText += static_cast<char>(key);
            }
            key = GetCharPressed();
        }

        // Handle backspace
        if (IsKeyPressed(KEY_BACKSPACE) && !inputText.empty()) {
            inputText.pop_back();
        }

        // Finish input on Enter key
        if (IsKeyPressed(KEY_ENTER)) {
            isEnteringText = false;
        }

        DrawPrompt(message, inputText, isEnteringText);
        EndDrawing();
    }

    return inputText;
}

void DrawPrompt(const std::string& message, const std::string& inputText,
                const bool& isEnteringText) {
    if (isEnteringText) {
        const int screenWidth = GetScreenWidth();
        const int screenHeight = GetScreenHeight();

        // Rectangle dimensions
        const int rectWidth = 400;
        const int rectHeight = 200;
        const int rectX = (screenWidth - rectWidth) / 2;
        const int rectY = (screenHeight - rectHeight) / 2;

        // Colors
        const Color borderColor = WHITE;
        const Color rectColor = DARKGRAY;
        const int borderThickness = 5;

        // Draw rectangle with border
        DrawRectangle(rectX - borderThickness, rectY - borderThickness,
                      rectWidth + 2 * borderThickness,
                      rectHeight + 2 * borderThickness, borderColor);
        DrawRectangle(rectX, rectY, rectWidth, rectHeight, rectColor);

        // Draw text
        const int textPadding = 20;
        const int messageY = rectY + textPadding;
        const int inputY = rectY + rectHeight / 2;

        DrawText(message.c_str(), rectX + textPadding, messageY, 20, WHITE);
        DrawText(inputText.c_str(), rectX + textPadding, inputY, 20, GREEN);
    }
}
