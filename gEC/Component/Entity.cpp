//
// Created by scion on 11/7/2022.
//

#include "Entity.h"
#include "../Windowing/Window.h"

gE::Entity::Entity(gE::Window* window, gE::Entity* parent, const char* name)
    : p_Window(window), p_Parent(parent), p_Name(name)
{

}

void gE::Entity::Destroy()
{
    for (Component::Component* const component: p_Components)
        component->FlagDeletion();
    GetWindow()->EntityManager.Destroy(this);
}

