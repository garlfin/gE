//
// Created by scion on 11/2/2022.
//

#pragma once

#include <vector>
#include "../Result.h"

namespace gE { class Window; }

namespace gE::Asset
{
    template<typename T, bool useWindow = true>
    class AssetManager
    {
    protected:
        std::vector<T*> p_Assets;
        std::vector<T*> p_Instantiation;
        std::vector<T*> p_Deletion;
        Window* p_Window;
        virtual void Destruct(T* t) { delete t; }
    public:
        explicit AssetManager(Window* window) : p_Assets(), p_Instantiation(), p_Deletion(), p_Window(window) {}
        ~AssetManager() {
            for(T* asset : p_Assets)
                delete asset;
        }

        template<typename I, typename... Args>
        I* Create(Args&&... args)
        {
            static_assert(std::is_base_of<T, I>::value, "'I' must inherit from 'T'");

            I* i;
            if constexpr(useWindow)
                i = new I(p_Window, std::forward<Args>(args)...);
            else
                i = new I(std::forward<Args>(args)...);
            p_Assets.push_back(i);
            p_Instantiation.push_back(i);

            return i;
        }

        gE::Result Remove(T* ptr)
        {
            auto index = std::find(p_Assets.begin(), p_Assets.end(), ptr);

            if (index == p_Assets.end()) return NOT_FOUND;

            p_Assets.erase(index);
            return OK;
        }

        gE::Result Destroy(T* ptr)
        {
            gE::Result result = Remove(ptr);

            if(!result) p_Deletion.push_back(ptr);

            return result;
        }

        T* Add(T* ptr)
        {
            p_Assets.push_back(ptr);
            return ptr;
        }

        virtual void OnUpdate(double delta)
        {
            for (T* const t : p_Deletion) Destruct(t);

            p_Instantiation.clear();
            p_Deletion.clear();
        }
    };
}