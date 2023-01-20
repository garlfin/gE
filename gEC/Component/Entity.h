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

    class Entity
    {
    protected:
        Entity(Window* window, Entity* parent, const char* name);

        Window const* OwningWindow;
        Entity* Parent;
        const char* Name;

        Component::Component** Components;
        uint32_t ComponentCount;

    public:
        void Destroy();

        template<class T>
        T* GetComponent() const
        {
            for (uint32_t i = 0; i < ComponentCount; i++)
                if (typeid(T) == typeid(*Components[i])) return (T*) Components[i];
            return nullptr;
        };

        [[nodiscard]] const char* const GetName()
        { return Name; }

        [[nodiscard]] Window* GetWindow()
        { return (Window*) OwningWindow; }

        [[nodiscard]] Entity* GetParent()
        { return Parent; }
    };

    class DynamicEntity final : public Entity
    {
    private:
        std::vector<Component::Component*> ComponentArray;
        void UpdateComponentData() { Components = ComponentArray.data(); ComponentCount = ComponentArray.size(); }

    public:
        explicit DynamicEntity(Window* window, Entity* parent = nullptr, const char* name = nullptr) : Entity(window, parent, name), ComponentArray() { Components = ComponentArray.data(); }

        template<typename T, typename I, typename... Args>
        T* CreateComponent(Component::ComponentManager<I>* manager, Args&&... args)
        {
            return AddComponent(manager->template Create<T>(this, std::forward<Args>(args)...));
        }

        template<typename T>
        T* AddComponent(T* c)
        {
            if(!GetComponent<T>())
                ComponentArray.push_back(c);
            UpdateComponentData();
            return c;
        }

        template<typename T>
        void RemoveComponent() { if(T* c = GetComponent<T>()) RemoveComponent(c); }
        void RemoveComponent(Component::Component* c) { std::erase(ComponentArray, c); UpdateComponentData(); }

        template<typename T>
        void DeleteComponent() { if(T* c = GetComponent<T>()) DeleteComponent(c); }
        void DeleteComponent(Component::Component* c) { c->FlagDeletion(); } // Yeah It can be static no i dont care
    };

    template<typename... ComponentPack>
    class StaticEntity : public Entity
    {
    private:
        Component::Component* _componentArray[sizeof...(ComponentPack)];
    protected:
        typedef StaticEntity<ComponentPack...> Base;
        explicit StaticEntity(Window* window, Entity* parent = nullptr, const char* name = nullptr) : Entity(window, parent, name) { Components = _componentArray; ComponentCount = sizeof...(ComponentPack); };
    };
}
