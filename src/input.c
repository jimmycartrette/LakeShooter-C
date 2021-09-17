#include "input.h"
#include "wasm4.h"

void Input_Initialize(struct Input *i)
{
    i->m_lastgamepad = 0;
    i->m_gamepad = 0;
}

void Input_Update(struct Input *i)
{

    i->m_lastgamepad = i->m_gamepad;
    i->m_gamepad = *GAMEPAD1;
}

bool Input_GamepadButtonPress(struct Input *i, const int button)
{
    if (button < 0 || button > 2)
    {
        return false;
    }
    switch (button)
    {
    case 1:
        return (i->m_lastgamepad & BUTTON_1) != BUTTON_1 && (i->m_gamepad & BUTTON_1) == BUTTON_1;
        break;
    case 2:
        return (i->m_lastgamepad & BUTTON_2) != BUTTON_2 && (i->m_gamepad & BUTTON_2) == BUTTON_2;
        break;
    default:
        return false;
    }
}

bool Input_GamepadButtonHeld(struct Input *i, const int button)
{
    if (button < 0 || button > 3)
    {
        return false;
    }
    switch (button)
    {
    case 1:
        return (i->m_gamepad & BUTTON_1) == BUTTON_1;
        break;
    case 2:
        return (i->m_gamepad & BUTTON_2) == BUTTON_2;
        break;
    default:
        return false;
    }
}

bool Input_GamepadLeftHeld(struct Input *i)
{
    return (i->m_gamepad & BUTTON_LEFT) == BUTTON_LEFT;
}

bool Input_GamepadRightHeld(struct Input *i)
{
    return (i->m_gamepad & BUTTON_RIGHT) == BUTTON_RIGHT;
}

bool Input_GamepadUpHeld(struct Input *i)
{
    return (i->m_gamepad & BUTTON_UP) == BUTTON_UP;
}
bool Input_GamepadDownHeld(struct Input *i)
{
    return (i->m_gamepad & BUTTON_DOWN) == BUTTON_DOWN;
}