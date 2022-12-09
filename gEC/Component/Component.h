//
// Created by scion on 11/10/2022.
//

#pragma once

#include <cstdint>
#include "../Asset/AssetManager.h"

namespace gE { class Entity; class Window; }

namespace gE::Component
{
    class Component
    {
    protected:
        Entity* p_Owner;
        bool p_Flagged;
        const bool p_Managed;
    public:
        explicit Component(Entity* owner, bool managed = false) : p_Owner(owner), p_Flagged(false), p_Managed(managed) {};
        virtual ~Component();

        [[nodiscard]] Window* GetWindow() const;
        [[nodiscard]] Entity* GetOwner() const { return p_Owner; }

        virtual void OnLoad() = 0;
        virtual void OnRender(double delta) = 0;
        virtual void OnUpdate(double delta) = 0;
        virtual void OnDestroy() = 0;

        void FlagDeletion() { p_Flagged = true; }
        [[nodiscard]] bool IsFlagged() const { return p_Flagged; }
        [[nodiscard]] bool IsManaged() const { return p_Managed; }
    };

    template<typename T>
    class   ComponentManager : public Asset::AssetManager<T>
    {
    private:
        typedef Asset::AssetManager<T> Base;
    public:
        ComponentManager() : Base()
        {
            static_assert(std::is_base_of<Component, T>::value, "'T' must inherit from 'Component'");
        };

        void OnUpdate(double delta) override
        {
            uint32_t deletionCount = 0;
            uint32_t s = Base::p_Assets.size();
            T** deletionArr = new T*[s];

            for(T* const component: Base::p_Instantiation)
                component->OnLoad();

            for (T* const component: Base::p_Assets)
                if (component->IsFlagged())
                {
                    deletionArr[deletionCount] = component;
                    deletionCount++;
                    component->OnDestroy();
                } else component->OnUpdate(delta);

            for (uint32_t i = 0; i < deletionCount; i++)
            {
                Base::Remove(deletionArr[i]);
                delete deletionArr[i];
            }

            delete[] deletionArr;
        }

        void OnRender(double delta) { for (T* const component : Base::p_Assets) component->OnRender(delta); }
    };
}