#pragma once

#include "Rapid/Core/Core.hpp"

#include "Rapid/ECS/Registry.hpp"

#include <cstdint>
#include <string>
#include <variant>
#include <functional>

namespace Rapid::Project
{

    ////////////////////////////////////////////////////////////////////////////////////
    // SceneTable // Note: Actual scene data
    ////////////////////////////////////////////////////////////////////////////////////
    struct SceneTable // Note: Scene data that's the same for 2D and 3D
    {
    public:
        // TODO: Physics Ticks (ex. 60-120)

    public:
        // Note: The setters are defined in the specific struct, because it needs to return the actual type.
    };

    ////////////////////////////////////////////////////////////////////////////////////
    // Scene2DTable
    ////////////////////////////////////////////////////////////////////////////////////
    struct Scene2DTable : public SceneTable
    {
    public:
        ECS::Registry2D Registry = {};

        // TODO: Physics layers

        // TODO: Camera

    public:
        // Setters
        inline Scene2DTable& SetRegistry(ECS::Registry2D&& registry) { Registry = std::move(registry); }
    };

    ////////////////////////////////////////////////////////////////////////////////////
    // Scene3DTable
    ////////////////////////////////////////////////////////////////////////////////////
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

}