#pragma once

#include "Obsidia/Core/Core.hpp"

#include "Obsidia/Project/VisualLayer.hpp"

#include <Obsidian/Core/Events.hpp>

#include <cstdint>
#include <string>
#include <vector>

namespace Ob::Project
{

    ////////////////////////////////////////////////////////////////////////////////////
    // SceneSpecification
    ////////////////////////////////////////////////////////////////////////////////////
    struct SceneSpecification
    {
    public:
        std::string Name = {};
        uint64_t UUID = 0; // Note: 0 means auto initialize

        std::vector<VisualLayerSpecification> VisualLayers = {};

    public:
        // Setters
        inline SceneSpecification& SetName(const std::string& name) { Name = name; return *this; }
        inline SceneSpecification& SetUUID(uint64_t uuid) { UUID = uuid; return *this; }

        inline SceneSpecification& SetVisualLayers(std::vector<VisualLayerSpecification>&& layers) { VisualLayers = std::move(layers); return *this; }
        inline SceneSpecification& AddVisualLayer(const VisualLayerSpecification& specs) { VisualLayers.emplace_back(specs); return *this; }
    };

    ////////////////////////////////////////////////////////////////////////////////////
    // Scene
    ////////////////////////////////////////////////////////////////////////////////////
    class Scene
    {
    public:
        // Constructor & Destructor
        Scene(const SceneSpecification& specs);
        ~Scene();

        // Methods
        void OnUpdate(float deltaTime);
        void OnRender();
        void OnEvent(const Obsidian::Event& e);

        // Getters
        inline SceneSpecification& GetSpecification() { return m_Specification; }

    private:
        // Private methods
        void CreateVisualLayers();

    private:
        SceneSpecification m_Specification;
        std::vector<VisualLayer> m_VisualLayers;
    };

}