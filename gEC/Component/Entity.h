//
// Created by scion on 11/7/2022.
//

#pragma once

#include "Component.h"
#include <vector>
#include <typeinfo>

namespace gE
{
    class Window;

    class Entity final
    {
    private:
        Window const* p_Window;
        Entity* p_Parent;
        const char* p_Name;
        std::vector<Component::Component*> p_Components;
    public:
        explicit Entity(Window* window, Entity* parent = nullptr, const char* name = nullptr);

        void Destroy();
        void AddComponent(Component::Component* c) { p_Components.push_back(c); }
        void RemoveComponent(Component::Component* c) { std::erase(p_Components, c); }
        void DeleteComponent(Component::Component* c) { c->FlagDeletion(); } // Yeah It can be static no i dont care

        template<class T>
        T* GetComponent() const
        {
            for (auto component : p_Components)
                if (typeid(T) == typeid(*component)) return (T*) component;
            return nullptr;
        };
        [[nodiscard]] const char* const GetName() { return p_Name; }
        [[nodiscard]] Window* GetWindow() { return (Window*) p_Window; }
        [[nodiscard]] Entity* GetParent() { return p_Parent; }
    };
}
