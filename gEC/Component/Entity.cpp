//
// Created by scion on 11/7/2022.
//

#include "Entity.h"
#include "../Windowing/Window.h"

gE::Entity::Entity(gE::Window* window, gE::Entity* parent, const char* name)
    : OwningWindow(window), Parent(parent), Name(name), Components(nullptr), ComponentCount(0), ChildrenCount(0), Layer(Layers::Default)
{

}

void gE::Entity::Destroy()
{
    for (uint32_t i = 0; i < ComponentCount; i++)
        Components[i]->FlagDeletion();
    GetWindow()->EntityManager.Destroy(this);
}

gE::Layers gE::operator&(const gE::Layers& a, const gE::Layers& b)
{
    return (Layers) ((uint8_t) a & (uint8_t) b);
}

gE::Layers gE::operator|(const gE::Layers& a, const gE::Layers& b)
{
    return (Layers) ((uint8_t) a | (uint8_t) b);
}

gE::Layers gE::operator|=(gE::Layers& a, const gE::Layers& b)
{
    return a = a | b;
}
