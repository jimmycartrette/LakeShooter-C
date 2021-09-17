#pragma once

#include <stdint.h>
#include <stdbool.h>

/*taken from here https: //github.com/jzeiber/wasm4-meteoroids/blob/main/src/src/input.h */
struct Input
{
    uint8_t m_lastgamepad;
    uint8_t m_gamepad;
};

void Input_Initialize(struct Input *i);
void Input_Update(struct Input *i);
bool Input_GamepadButtonPress(struct Input *i, const int button);
bool Input_GamepadButtonHeld(struct Input *i, const int button);
bool Input_GamepadLeftHeld(struct Input *i);
bool Input_GamepadRightHeld(struct Input *i);
bool Input_GamepadUpHeld(struct Input *i);
bool Input_GamepadDownHeld(struct Input *i);