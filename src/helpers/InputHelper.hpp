#ifndef INPUTHELPER_H
#define INPUTHELPER_H

#include <raylib.h>

#include <string>

// Function to capture text input using Raylib
std::string Prompt(const std::string& message);
void DrawPrompt(const std::string& message, const std::string& inputText,
                const bool& isEnteringText);

#endif
