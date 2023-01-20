//
// Created by scion on 11/10/2022.
//
#include "Component.h"
#include "Entity.h"

gE::Window* gE::Component::Component::GetWindow() const
{
    return p_Owner->GetWindow();
}

gE::Component::Component::~Component()
{
    if(typeid(*p_Owner) == typeid(DynamicEntity)) ((DynamicEntity*) p_Owner)->DeleteComponent(this);
}

