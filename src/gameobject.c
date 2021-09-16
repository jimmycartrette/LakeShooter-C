#include "gameobject.h"

void GameObject_Initialize(struct GameObject *o)
{
    o->m_type = OBJECT_NONE;
    o->m_alive = false;
}