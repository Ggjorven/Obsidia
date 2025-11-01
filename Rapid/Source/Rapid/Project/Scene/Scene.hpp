#pragma once

#include "Rapid/Core/Core.hpp"
#include "Rapid/Core/Logger.hpp"

#include "Rapid/ECS/Registry.hpp"

#include "Rapid/Project/Events.hpp"
#include "Rapid/Project/Renderer/Renderer.hpp"

#include "Rapid/Project/Scene/SceneSpecification.hpp"
#include "Rapid/Project/Scene/Scene2D.hpp"
#include "Rapid/Project/Scene/Scene3D.hpp"

#include <Obsidian/Core/Events.hpp>

#include <cstdint>
#include <string>
#include <vector>
#include <memory>
#include <variant>
#include <functional>
#include <type_traits>

namespace Rapid::Project
{

    ////////////////////////////////////////////////////////////////////////////////////
    // Scene
    ////////////////////////////////////////////////////////////////////////////////////
    class Scene // Note: A wrapper around different scene types.
    {
    public:
        // Constructor & Destructor
        inline Scene(const SceneSpecification& specs, Scene2DTable&& table)
            : m_Scene(std::in_place_type<Scene2D>, specs, std::move(table)) {}
        inline Scene(const SceneSpecification& specs, Scene3DTable&& table)
            : m_Scene(std::in_place_type<Scene3D>, specs, std::move(table)) {}
        ~Scene() = default;

        // Methods
        inline void OnUpdate(float deltaTime) { std::visit([&](auto&& obj) -> void { obj.OnUpdate(deltaTime); }, m_Scene); }
        inline void OnEvent(const Event& e) { std::visit([&](auto&& obj) -> void { obj.OnEvent(e); }, m_Scene); }

        // Getters
        inline const SceneSpecification& GetSpecification() const { return std::visit([&](auto&& obj) -> const SceneSpecification& { return obj.GetSpecification(); }, m_Scene); }
        inline bool Is2D() const { return GetSpecification().Is2D(); }
        inline bool Is3D() const { return GetSpecification().Is3D(); }

        inline const Scene2D& Get2D() { RP_ASSERT(Is2D(), "Scene must be 2D."); return std::visit([&](auto&& obj) -> const Scene2D& { if constexpr (std::is_same_v<std::decay_t<decltype(obj)>, Scene2D>) return obj; else return *static_cast<const Scene2D*>(nullptr); }, m_Scene); }
        inline const Scene3D& Get3D() { RP_ASSERT(Is3D(), "Scene must be 3D."); return std::visit([&](auto&& obj) -> const Scene3D& { if constexpr (std::is_same_v<std::decay_t<decltype(obj)>, Scene3D>) return obj; else return *static_cast<const Scene3D*>(nullptr); }, m_Scene); }

    private:
        std::variant<Scene2D, Scene3D> m_Scene;
    };

}