//
// Created by scion on 11/19/2022.
//

#pragma once

#include "../GLAsset.h"
#include "GLAD/glad.h"

namespace gE
{
    enum class BufferTarget : uint64_t
    {
        ATOMIC = GL_ATOMIC_COUNTER_BUFFER,
        TRANSFORM_FEEDBACK = GL_TRANSFORM_FEEDBACK_BUFFER,
        UNIFORM = GL_UNIFORM_BUFFER,
        SHADER_STORAGE = GL_SHADER_STORAGE_BUFFER
    };

    template<class T>
    class Buffer : public Asset::GLAsset
    {
    private:
        uint32_t p_Count;
        uint32_t p_TMultiplier;
    public:
        explicit Buffer(Window* window, uint32_t count = 1, uint32_t sizeMultiplier = 1, T* data = nullptr) : GLAsset::GLAsset(window), p_Count(count), p_TMultiplier(sizeMultiplier)
        {
            glCreateBuffers(1, &ID);
            glNamedBufferStorage(ID, sizeof(T) * count * sizeMultiplier, data, GL_DYNAMIC_STORAGE_BIT);
        }

        template<class I>
        void ReplaceData(I* data, uint32_t count = 1, uint32_t offset = 0, uint32_t sizeMultiplier = 0)
        {
            if (!count || !data) return;
            glNamedBufferSubData(ID, offset * p_TMultiplier, sizeof(I) * count * (sizeMultiplier ?: p_TMultiplier), data);
        }

        uint32_t Bind(uint32_t slot, BufferTarget target)
        {
            glBindBufferBase((GLenum) target, slot, ID);
            return slot;
        }

        void Bind(BufferTarget target)
        {
            glBindBuffer((GLenum) target, ID);
        }

        ~Buffer() override
        {
            glDeleteBuffers(1, &ID);
        }

        [[nodiscard]] uint32_t GetByteCount() const
        {
            return sizeof(T) * p_Count * p_TMultiplier;
        }

        [[nodiscard]] uint32_t GetOffset(uint32_t offset = 1) const
        {
            return sizeof(T) * offset * p_TMultiplier;
        }

        [[nodiscard]] uint32_t GetCount() const
        {
            return p_Count;
        }
    };
}

