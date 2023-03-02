//
// Created by scion on 11/7/2022.
//

#pragma once

#include "Component.h"
#include <vector>
#include <typeinfo>
#include <cstring>

namespace gE
{
    class Window;

    enum class Layers : uint8_t
    {
        Default = 0b00000001,
        One     = 0b00000010,
        Two     = 0b00000100,
        Three   = 0b00001000,
        Four    = 0b00010000,
        Five    = 0b00100000,
        Six     = 0b01000000,
        Seven   = 0b10000000,
        All = UINT8_MAX
    };

   uint8_t operator &(Layers const& a, Layers const& b);

    class Entity
    {
    protected:
        Entity(Window* window, Entity* parent, const char* name);

        Window const* OwningWindow;
        Entity* Parent;
        const char* Name;

        Entity** Children;
        uint32_t ChildrenCount;

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

        Entity* GetChild(const char* name)
        {
            for (uint32_t i = 0; i < ChildrenCount; i++)
                if(!strcmp(Children[i]->Name, name)) return Children[i];
            return nullptr;
        }

        [[nodiscard]] const char* const GetName()
        { return Name; }

        [[nodiscard]] Window* GetWindow()
        { return (Window*) OwningWindow; }

        [[nodiscard]] Entity* GetParent()
        { return Parent; }

        Layers Layer;
    };

    class DynamicEntity final : public Entity
    {
    private:
        std::vector<Component::Component*> _componentArray;
        std::vector<Entity*> _childrenArray;

        void UpdateComponentData() { Components = _componentArray.data(); ComponentCount = _componentArray.size(); }
        void UpdateChildrenData() { Children = _childrenArray.data(); ChildrenCount = _childrenArray.size(); }

    public:
        explicit DynamicEntity(Window* window, DynamicEntity* parent = nullptr, const char* name = nullptr) : Entity(window, parent, name), _componentArray(), _childrenArray() { UpdateChildrenData(); UpdateComponentData(); if(parent)
                parent->AddChild(this); }

        template<typename T, typename I, typename... Args>
        T* CreateComponent(Component::ComponentManager<I>* manager, Args&&... args)
        {
            return AddComponent(manager->template Create<T>(this, std::forward<Args>(args)...));
        }

        template<typename T>
        T* AddComponent(T* c)
        {
            if(!GetComponent<T>())
                _componentArray.push_back(c);
            UpdateComponentData();
            return c;
        }

        template<typename T>
        void RemoveComponent() { if(T* c = GetComponent<T>()) RemoveComponent(c); }
        void RemoveComponent(Component::Component* c) { std::erase(_componentArray, c); UpdateComponentData(); }

        template<typename T>
        void DeleteComponent() { if(T* c = GetComponent<T>()) DeleteComponent(c); }
        void DeleteComponent(Component::Component* c) { c->FlagDeletion(); } // Yeah It can be static no i dont care

        void AddChild(Entity* entity) { _childrenArray.push_back(entity); UpdateChildrenData(); }
        void Remove(Entity* entity) { std::erase(_childrenArray, entity); UpdateChildrenData();  }
    };

    template<int TChildrenCount, typename... ComponentPack>
    class StaticEntity : public Entity
    {
    private:
        Component::Component* _componentArray[sizeof...(ComponentPack)];
        Entity* _childrenArray[TChildrenCount] {};
    protected:
        typedef StaticEntity<TChildrenCount, ComponentPack...> Base;
        explicit StaticEntity(Window* window, Entity* parent = nullptr, const char* name = nullptr) : Entity(window, parent, name) { Components = _componentArray; ComponentCount = sizeof...(ComponentPack); Children = _childrenArray; ChildrenCount = TChildrenCount; }
    };
}
