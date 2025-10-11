#pragma once

#include "Obsidia/Core/Core.hpp"

#include <cstdint>

namespace Ob::Project
{

    class Scene;

    ////////////////////////////////////////////////////////////////////////////////////
    // VisualLayerSpecification
    ////////////////////////////////////////////////////////////////////////////////////
    struct VisualLayerSpecification
    {
    public:
        uint8_t Level = 0; // Note: 0, means auto initialize

    public:
        // Setters
        inline constexpr VisualLayerSpecification& SetLevel(uint8_t level) { Level = level; return *this; }
    };

    ////////////////////////////////////////////////////////////////////////////////////
    // VisualLayer
    ////////////////////////////////////////////////////////////////////////////////////
    class VisualLayer
    {
    public:
        // Constructor & Destructor
        VisualLayer(const VisualLayerSpecification& specs);
        ~VisualLayer();

        // Getters
        inline const VisualLayerSpecification& GetSpecification() { return m_Specification; }

    private:
        // Private methods

    private:
        VisualLayerSpecification m_Specification;
        // TODO: Renderpass

        friend class Scene;
    };

}