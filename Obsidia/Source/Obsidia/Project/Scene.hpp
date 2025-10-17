#pragma once

#include "Obsidia/Core/Core.hpp"
#include "Obsidia/Core/Logger.hpp"

#include "Obsidia/ECS/Registry.hpp"

#include "Obsidia/Project/VisualLayer.hpp"

#include <Obsidian/Core/Events.hpp>

#include <cstdint>
#include <string>
#include <vector>
#include <memory>
#include <variant>
#include <functional>
#include <type_traits>

namespace Ob::Project
{

    class Project;

    ////////////////////////////////////////////////////////////////////////////////////
    // SceneTable // Note: Actual scene data
    ////////////////////////////////////////////////////////////////////////////////////
    struct SceneTable // Note: Scene data that's the same for 2D and 3D
    {
    public:
        std::vector<VisualLayerSpecification> VisualLayers = {};

        // TODO: Physics Ticks

    public:
        // Note: The setters are defined in the specific struct, because it needs to return the actual type.
    };

    struct Scene2DTable : public SceneTable
    {
    public:
        ECS::Registry2D Registry = {};

        // TODO: Physics layers

        // TODO: Camera

    public:
        // Setters
        inline Scene2DTable& SetRegistry(ECS::Registry2D&& registry) { Registry = std::move(registry); }

        inline Scene2DTable& SetVisualLayers(std::vector<VisualLayerSpecification>&& layers) { VisualLayers = std::move(layers); return *this; }
        inline Scene2DTable& AddVisualLayer(const VisualLayerSpecification& specs) { VisualLayers.emplace_back(specs); return *this; }
    };

    struct Scene3DTable : public SceneTable
    {
    public:
        ECS::Registry3D Registry = {};

        // TODO: Physics layers

        // TODO: Camera
        // TODO: LightingModel
        // TODO: MSAA

    public:
        // Setters
        inline Scene3DTable& SetRegistry(ECS::Registry3D&& registry) { Registry = std::move(registry); }

        inline Scene3DTable& SetVisualLayers(std::vector<VisualLayerSpecification>&& layers) { VisualLayers = std::move(layers); return *this; }
        inline Scene3DTable& AddVisualLayer(const VisualLayerSpecification& specs) { VisualLayers.emplace_back(specs); return *this; }
    };

    ////////////////////////////////////////////////////////////////////////////////////
    // SceneSpecification // Note: General data
    ////////////////////////////////////////////////////////////////////////////////////
    struct SceneSpecification
    {
    public:
        using Load2DFn = std::function<Scene2DTable(const SceneSpecification& specs)>;
        using Load3DFn = std::function<Scene3DTable(const SceneSpecification& specs)>;
    public:
        std::string Name = {};
        uint64_t UUID = 0; // Note: 0 means auto initialize

        std::variant<Load2DFn, Load3DFn> LoadSceneFn = [](const SceneSpecification& specs) -> Scene2DTable { (void)specs; return Scene2DTable(); };

    public:
        // Setters
        inline SceneSpecification& SetName(const std::string& name) { Name = name; return *this; }
        inline SceneSpecification& SetUUID(uint64_t uuid) { UUID = uuid; return *this; }

        inline SceneSpecification& SetLoadSceneFn(const Load2DFn& fn) { LoadSceneFn = fn; return *this; }
        inline SceneSpecification& SetLoadSceneFn(const Load3DFn& fn) { LoadSceneFn = fn; return *this; }

        // Getters
        inline bool Is2D() const { return std::holds_alternative<Load2DFn>(LoadSceneFn); }
        inline bool Is3D() const { return std::holds_alternative<Load3DFn>(LoadSceneFn); }
    };

    ////////////////////////////////////////////////////////////////////////////////////
    // Scene
    ////////////////////////////////////////////////////////////////////////////////////
    class Scene // Note: A base scene class with functionality used in all scenes
    {
    public:
        // Constructor & Destructor
        Scene(const SceneSpecification& specs, const SceneTable& table);
        ~Scene();

        // Methods
        virtual void OnUpdate(float deltaTime) = 0;
        virtual void OnRender() = 0;
        virtual void OnEvent(const Obsidian::Event& e) = 0;

        // Getters
        inline const SceneSpecification& GetSpecification() { return m_Specification; }

    private:
        // Private methods
        void CreateVisualLayers();

    private:
        SceneSpecification m_Specification;
        SceneTable m_GlobalTable;

        std::vector<VisualLayer> m_VisualLayers;
    };

}