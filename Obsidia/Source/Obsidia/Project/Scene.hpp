#pragma once

#include "Obsidia/Core/Core.hpp"
#include "Obsidia/Core/Logger.hpp"

#include "Obsidia/ECS/Registry.hpp"

#include "Obsidia/Project/VisualLayer.hpp"

#include <Obsidian/Core/Events.hpp>

#include <cstdint>
#include <string>
#include <vector>
#include <variant>
#include <functional>
#include <type_traits>

namespace Ob::Project
{

    ////////////////////////////////////////////////////////////////////////////////////
    // SceneTable // Note: Actual scene data
    ////////////////////////////////////////////////////////////////////////////////////
    struct Scene2DTable
    {
    public:
        ECS::Registry2D Registry = {};

        std::vector<VisualLayerSpecification> VisualLayers = {};
        // TODO: Physics layers

        // TODO: Camera
        // TODO: LightingModel
        // TODO: MSAA

        // TODO: Physics Ticks

    public:
        // Setters
        inline Scene2DTable& SetRegistry(ECS::Registry2D&& registry) { Registry = std::move(registry); }

        inline Scene2DTable& SetVisualLayers(std::vector<VisualLayerSpecification>&& layers) { VisualLayers = std::move(layers); return *this; }
        inline Scene2DTable& AddVisualLayer(const VisualLayerSpecification& specs) { VisualLayers.emplace_back(specs); return *this; }
    };

    struct Scene3DTable
    {
    public:
        ECS::Registry3D Registry = {};

        std::vector<VisualLayerSpecification> VisualLayers = {};
        // TODO: Physics layers

        // TODO: Camera
        // TODO: LightingModel
        // TODO: MSAA

        // TODO: Physics Ticks

    public:
        // Setters
    };

    struct SceneTable
    {
    public:
        std::variant<Scene2DTable, Scene3DTable> Table;

    public:
        // Getters
        inline std::vector<VisualLayerSpecification>& GetVisualLayerSpecifications() { return std::visit([](auto&& obj) -> std::vector<VisualLayerSpecification>&{ return obj.VisualLayers; }, Table); }
    };

    ////////////////////////////////////////////////////////////////////////////////////
    // SceneSpecification // Note: General data
    ////////////////////////////////////////////////////////////////////////////////////
    struct SceneSpecification
    {
    public:
        std::string Name = {};
        uint64_t UUID = 0; // Note: 0 means auto initialize

        std::function<SceneTable()> LoadSceneFn = []() -> SceneTable { return SceneTable(); };

    public:
        // Setters
        inline SceneSpecification& SetName(const std::string& name) { Name = name; return *this; }
        inline SceneSpecification& SetUUID(uint64_t uuid) { UUID = uuid; return *this; }

        inline SceneSpecification& SetLoadSceneFn(const std::function<SceneTable&&()>& fn) { LoadSceneFn = fn; return *this; }
    };

    ////////////////////////////////////////////////////////////////////////////////////
    // Scene
    ////////////////////////////////////////////////////////////////////////////////////
    class Scene
    {
    public:
        // Constructor & Destructor
        Scene(const SceneSpecification& specs, SceneTable&& table);
        ~Scene();

        // Methods
        void OnUpdate(float deltaTime);
        void OnRender();
        void OnEvent(const Obsidian::Event& e);

        // Getters
        inline const SceneSpecification& GetSpecification() { return m_Specification; }

    private:
        // Private methods
        void CreateVisualLayers();

    private:
        SceneSpecification m_Specification;
        SceneTable m_Table;

        std::vector<VisualLayer> m_VisualLayers;
    };

}