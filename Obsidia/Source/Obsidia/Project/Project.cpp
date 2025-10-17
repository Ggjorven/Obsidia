#include "obpch.h"
#include "Project.hpp"

#include "Obsidia/Core/Core.hpp"
#include "Obsidia/Core/Logger.hpp"

#include "Obsidia/Project/Scene2D.hpp"
#include "Obsidia/Project/Scene3D.hpp"

#include <ranges>

namespace Ob::Project
{

    ////////////////////////////////////////////////////////////////////////////////////
    // Adders
    ////////////////////////////////////////////////////////////////////////////////////
    void SceneCollection::AddSpecification(const SceneSpecification& specs)
    {
        SceneSpecByName[specs.Name] = specs;
        SceneSpecByUUID[specs.UUID] = specs;
    }

    void SceneCollection::AddScene(std::shared_ptr<Scene> scene)
    {
        OBSIDIA_ASSERT(scene, "Scene must not be null.");

        const SceneSpecification& specs = scene->GetSpecification();

        if (!SpecContains(specs.UUID)) [[unlikely]]
        {
            Logger::Warning("[SceneCollection] Trying to add a scene, that hasn't previously been added as specification. Adding it now.");
            SceneSpecByName[specs.Name] = specs;
            SceneSpecByUUID[specs.UUID] = specs;
        }

        SceneByUUID[specs.UUID] = scene;
        SceneByName[specs.Name] = scene;
    }

    ////////////////////////////////////////////////////////////////////////////////////
    // Getters
    ////////////////////////////////////////////////////////////////////////////////////
    bool SceneCollection::SpecContains(const SceneID& sceneIdentifier) const
    {
        return std::visit(
            [&](auto&& obj) -> bool
            {
                if constexpr (std::is_same_v<std::decay_t<decltype(obj)>, uint64_t>)
                {
                    return SceneSpecByUUID.contains(obj);
                }
                else if constexpr (std::is_same_v<std::decay_t<decltype(obj)>, std::string>)
                {
                    return SceneSpecByName.contains(obj);
                }
            },
            sceneIdentifier
        );
    }

    bool SceneCollection::SceneLoaded(const SceneID& sceneIdentifier) const
    {
        return std::visit(
            [&](auto&& obj) -> bool
            {
                if constexpr (std::is_same_v<std::decay_t<decltype(obj)>, uint64_t>)
                {
                    return SceneByUUID.contains(obj);
                }
                else if constexpr (std::is_same_v<std::decay_t<decltype(obj)>, std::string>)
                {
                    return SceneByName.contains(obj);
                }
            },
            sceneIdentifier
        );
    }

    std::expected<SceneSpecification, ErrorCode> SceneCollection::GetSpecification(const SceneID& sceneIdentifier)
    {
        return std::visit(
            [&](auto&& obj) -> std::expected<SceneSpecification, ErrorCode>
            {
                if constexpr (std::is_same_v<std::decay_t<decltype(obj)>, uint64_t>)
                {
                    if (!SceneSpecByUUID.contains(obj))
                        return std::unexpected(ErrorCode::UUIDToSpecificationNotFound);

                    return SceneSpecByUUID[obj];
                }
                else if constexpr (std::is_same_v<std::decay_t<decltype(obj)>, std::string>)
                {
                    if (!SceneSpecByName.contains(obj))
                        return std::unexpected(ErrorCode::NameToSpecificationNotFound);

                    return SceneSpecByName[obj];
                }
            },
            sceneIdentifier
        );
    }

    ////////////////////////////////////////////////////////////////////////////////////
    // Constructor & Destructor
    ////////////////////////////////////////////////////////////////////////////////////
    Project::Project(const ProjectSpecification& specs)
        : m_Specification(specs)
    {
        Logger::Info("[Project] Initializing project named: \"{0}\"", m_Specification.Name);

        ProcessScenes();

        // Load start scene
        {
            auto startScene = LoadScene(m_Specification.StartScene);
            if (startScene.has_value())
                m_Scenes.AddScene(startScene.value());
            else
                Logger::Error("Failed to load scene. Error code: {0}", Nano::Enum::Name(startScene.error()));
        }
    }

    Project::~Project()
    {
    }

    ////////////////////////////////////////////////////////////////////////////////////
    // Methods
    ////////////////////////////////////////////////////////////////////////////////////
    void Project::OnUpdate(float deltaTime)
    {
    }

    void Project::OnRender()
    {
    }

    void Project::OnEvent(const Obsidian::Event& e)
    {
    }

    ////////////////////////////////////////////////////////////////////////////////////
    // Private methods
    ////////////////////////////////////////////////////////////////////////////////////
    void Project::ProcessScenes()
    {
        Logger::Info("[Project] Preprocessing scenes to ensure validity.");

        // UUID & Name verification
        {
            std::unordered_set<uint64_t> UUIDs;
            std::unordered_set<std::string> Names;

            for (auto& scene : m_Specification.Scenes)
            {
                if (scene.UUID == 0) // Note: 0 means auto-initialize
                    scene.UUID = Nano::Random::Random::UInt64();

                if (UUIDs.contains(scene.UUID))
                {
                    Logger::Error("[Project] Scene with name: \"{0}\" has a conflicting uuid with another scene ({1}). Resetting UUID, this can cause serious issues.", scene.Name, scene.UUID);
                    scene.UUID = Nano::Random::Random::UInt64();
                }
                if (Names.contains(scene.Name))
                {
                    Logger::Error("[Project] Scene with name: \"{0}\" has a conflicting name with another scene. Resetting name, this can cause serious issues.", scene.Name);
                    scene.Name = "Scene-" + std::to_string(scene.UUID);
                }

                UUIDs.insert(scene.UUID);
                Names.insert(scene.Name);
            }
        }

        // Make sure there is a scene
        {
            if (m_Specification.Scenes.empty())
            {
                Logger::Warning("[Project] No scene specifications passed, creating default scene.");

                const uint64_t uuid = Nano::Random::Random::UInt64();
                m_Specification.Scenes.emplace_back(SceneSpecification()
                    .SetUUID(uuid)
                    .SetName("Scene-" + std::to_string(uuid))
                );
                m_Specification.StartScene = uuid;
            }
        }

        // Move specifications into maps
        {
            for (const auto& scene : m_Specification.Scenes)
                m_Scenes.AddSpecification(scene);
        }

        // Verify start UUID
        {
            if (!m_Scenes.SpecContains(m_Specification.StartScene))
                Logger::Warning("[Project] StartScene id has no corresponding scene. Resetting to first scene.");
        }
    }

    std::expected<std::shared_ptr<Scene>, ErrorCode> Project::LoadScene(const SceneID& sceneIdentifier)
    {
        // Load the specification
        const auto sceneSpec = m_Scenes.GetSpecification(sceneIdentifier);

        // Handle error
        if (!sceneSpec.has_value())
            return std::unexpected(sceneSpec.error());

        Logger::Info("[Project] Loading scene: \"{0}\" with id: {1}", sceneSpec.value().Name, sceneSpec.value().UUID);

        // Load the scene
        return std::visit(
            [&](auto&& obj) -> std::expected<std::shared_ptr<Scene>, ErrorCode>
            {
                // Make sure we can load
                if (obj == nullptr)
                    return std::unexpected(ErrorCode::NoLoadFunction);

                if constexpr (std::is_same_v<std::decay_t<decltype(obj)>, SceneSpecification::Load2DFn>)
                {
                    Scene2DTable table = obj(sceneSpec.value());
                    return std::make_shared<Scene2D>(sceneSpec.value(), std::move(table));
                }
                else if constexpr (std::is_same_v<std::decay_t<decltype(obj)>, SceneSpecification::Load3DFn>)
                {
                    Scene3DTable table = obj(sceneSpec.value());
                    return std::make_shared<Scene3D>(sceneSpec.value(), std::move(table));
                }

            },
            sceneSpec.value().LoadSceneFn
        );
    }

    std::expected<void, ErrorCode> Project::UnloadScene(const SceneID& sceneIdentifier)
    {
        // Load the specification
        const auto sceneSpec = m_Scenes.GetSpecification(sceneIdentifier);

        // Handle error
        if (!sceneSpec.has_value())
            return std::unexpected(sceneSpec.error());

        Logger::Info("[Project] Unloading scene: \"{0}\" with id: {1}", sceneSpec.value().Name, sceneSpec.value().UUID);

        // Erase scene, if no more references shared_ptr should destroy the scene
        {
            std::optional<ErrorCode> errorCode = {};

            // UUID
            if (m_Scenes.SceneByUUID.contains(sceneSpec.value().UUID)) [[likely]]
                m_Scenes.SceneByUUID.erase(sceneSpec.value().UUID);
            else [[unlikely]]
                errorCode = ErrorCode::UUIDToSceneNotFound;

            // Name
            if (m_Scenes.SceneByName.contains(sceneSpec.value().Name)) [[likely]]
                m_Scenes.SceneByName.erase(sceneSpec.value().Name);
            else [[unlikely]]
            {
                if (errorCode.has_value())
                    return std::unexpected(ErrorCode::SceneNotFound); // Both UUID and Name not found
                return std::unexpected(ErrorCode::NameToSceneNotFound);
            }
        }

        return {};
    }

}