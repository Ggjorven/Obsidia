#include "obpch.h"
#include "Project.hpp"

#include "Obsidia/Core/Core.hpp"
#include "Obsidia/Core/Logger.hpp"

#include <ranges>

namespace Ob::Project
{

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
            {
                m_SceneByName[startScene.value()->GetSpecification().Name] = startScene.value();
                m_SceneByUUID[startScene.value()->GetSpecification().UUID] = startScene.value();
            }
            else
            {
                Logger::Error("Failed to load scene. Error code: {0}", Nano::Enum::Name(startScene.error()));
            }
        }
    }

    Project::~Project()
    {
    }

    ////////////////////////////////////////////////////////////////////////////////////
    // Methods
    ////////////////////////////////////////////////////////////////////////////////////
    std::expected<std::shared_ptr<Scene>, Project::LoadSceneError> Project::LoadScene(const SceneID& sceneIdentifier)
    {
        // Load the specification
        const auto sceneSpec = std::visit(
            [&](auto&& obj) -> std::expected<const SceneSpecification*, LoadSceneError>
            {
                if constexpr (std::is_same_v<std::decay_t<decltype(obj)>, uint64_t>)
                {
                    if (!m_SceneSpecByUUID.contains(obj))
                        return std::unexpected(LoadSceneError::UUIDNotFound);

                    return m_SceneSpecByUUID[obj];
                }
                else if constexpr (std::is_same_v<std::decay_t<decltype(obj)>, std::string>)
                {
                    if (!m_SceneSpecByName.contains(obj))
                        return std::unexpected(LoadSceneError::NameNotFound);

                    return m_SceneSpecByName[obj];
                }
            },
            sceneIdentifier
        );

        // Handle error
        if (!sceneSpec.has_value())
            return std::unexpected(sceneSpec.error());

        Logger::Info("[Project] Loading scene: \"{0}\" with id: {1}", sceneSpec.value()->Name, sceneSpec.value()->UUID);

        // Load the scene
        return std::visit(
            [&](auto&& obj) -> std::expected<std::shared_ptr<Scene>, LoadSceneError>
            {
                // Make sure we can load
                if (obj == nullptr)
                    return std::unexpected(LoadSceneError::NoLoadFunction);

                if constexpr (std::is_same_v<std::decay_t<decltype(obj)>, SceneSpecification::Load2DFn>)
                {
                    Scene2DTable table = obj(*sceneSpec.value());
                    return std::make_shared<Scene2D>(*sceneSpec.value(), std::move(table));
                }
                else if constexpr (std::is_same_v<std::decay_t<decltype(obj)>, SceneSpecification::Load3DFn>)
                {
                    Scene3DTable table = obj(*sceneSpec.value());
                    return std::make_shared<Scene3D>(*sceneSpec.value(), std::move(table));
                }

            },
            sceneSpec.value()->LoadSceneFn
        );
    }

    std::expected<void, Project::UnloadSceneError> Project::UnloadScene(const SceneID& sceneIdentifier)
    {
        // Load the specification
        const auto sceneSpec = std::visit(
            [&](auto&& obj) -> std::expected<const SceneSpecification*, UnloadSceneError>
            {
                if constexpr (std::is_same_v<std::decay_t<decltype(obj)>, uint64_t>)
                {
                    if (!m_SceneSpecByUUID.contains(obj))
                        return std::unexpected(UnloadSceneError::UUIDToSpecificationNotFound);

                    return m_SceneSpecByUUID[obj];
                }
                else if constexpr (std::is_same_v<std::decay_t<decltype(obj)>, std::string>)
                {
                    if (!m_SceneSpecByName.contains(obj))
                        return std::unexpected(UnloadSceneError::NameToSpecificationNotFound);

                    return m_SceneSpecByName[obj];
                }
            },
            sceneIdentifier
        );

        // Handle error
        if (!sceneSpec.has_value())
            return std::unexpected(sceneSpec.error());

        Logger::Info("[Project] Unloading scene: \"{0}\" with id: {1}", sceneSpec.value()->Name, sceneSpec.value()->UUID);

        // Erase scene, if no more references shared_ptr should destroy the scene
        {
            std::optional<UnloadSceneError> errorCode = {};

            // UUID
            if (m_SceneByUUID.contains(sceneSpec.value()->UUID)) [[likely]]
                m_SceneByUUID.erase(sceneSpec.value()->UUID);
            else [[unlikely]]
                errorCode = UnloadSceneError::UUIDToSceneNotFound;

            // Name
            if (m_SceneByName.contains(sceneSpec.value()->Name)) [[likely]]
                m_SceneByName.erase(sceneSpec.value()->Name);
            else [[unlikely]]
            {
                if (errorCode.has_value())
                    return std::unexpected(UnloadSceneError::SceneNotFound); // Both UUID and Name not found
                return std::unexpected(UnloadSceneError::NameToSceneNotFound);
            }
        }

        return {};
    }

    ////////////////////////////////////////////////////////////////////////////////////
    // Private methods
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
            for (auto& scene : m_Specification.Scenes)
            {
                m_SceneSpecByName[scene.Name] = &scene;
                m_SceneSpecByUUID[scene.UUID] = &scene;
            }
        }

        // Verify start UUID // TODO: Verify all UUIDs?
        {
            std::visit(
                [&](auto&& obj)
                {
                    if constexpr (std::is_same_v<std::decay_t<decltype(obj)>, uint64_t>)
                    {
                        if (!m_SceneSpecByUUID.contains(obj))
                        {
                            Logger::Warning("[Project] StartScene has ID: {0}, but there is no corresponding scene. Resetting to first scene.", obj);
                            m_Specification.StartScene = m_Specification.Scenes[0].UUID;
                        }
                    }
                    else if constexpr (std::is_same_v<std::decay_t<decltype(obj)>, std::string>)
                    {
                        if (!m_SceneSpecByName.contains(obj))
                        {
                            Logger::Warning("[Project] StartScene has name: \"{0}\", but there is no corresponding scene. Resetting to first scene.", obj);
                            m_Specification.StartScene = m_Specification.Scenes[0].Name;
                        }
                    }
                },
                m_Specification.StartScene
            );
        }
    }

}