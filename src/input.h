#pragma once

#include <stdint.h>
#include <stdbool.h>

struct Input
{
    uint8_t m_lastgamepad;
    uint8_t m_gamepad;
};

void Input_Initialize(struct Input *i);
void Input_Update(struct Input *i);
bool Input_GamepadButtonPress(struct Input *i, const int button);
bool Input_GamepadButtonDown(struct Input *i, const int button);
bool Input_GamepadLeftDown(struct Input *i);
bool Input_GamepadRightDown(struct Input *i);
bool Input_GamepadUpDown(struct Input *i);