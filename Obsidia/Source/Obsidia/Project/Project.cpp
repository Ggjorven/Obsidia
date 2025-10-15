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
        InitializeStart();
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
                    scene.UUID = 0; // TODO: Random UUID

                if (UUIDs.contains(scene.UUID))
                {
                    Logger::Error("[Project] Scene with name: \"{0}\" has a conflicting uuid with another scene ({1}). Resetting UUID, this can cause serious issues.", scene.Name, scene.UUID);
                    scene.UUID = 0; // TODO: Random UUID
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

                const uint64_t uuid = 0; // TODO: Random UUID
                m_Specification.Scenes.emplace_back(SceneSpecification()
                    .SetUUID(uuid)
                    .SetName("Scene-" + std::to_string(uuid))
                );
                m_Specification.StartScene = uuid;
            }
        }

        // Move into maps
        {
            for (auto& scene : m_Specification.Scenes)
            {
                m_SceneByName[scene.Name] = &scene;
                m_SceneByUUID[scene.UUID] = &scene;
            }
        }

        // Verify start UUID
        {
            std::visit(
                [&](auto&& obj)
                {
                    if constexpr (std::is_same_v<std::decay_t<decltype(obj)>, uint64_t>)
                    {
                        if (!m_SceneByUUID.contains(obj))
                        {
                            Logger::Warning("[Project] StartScene has ID: {0}, but there is no corresponding scene. Resetting to first scene.", obj);
                            m_Specification.StartScene = m_Specification.Scenes[0].UUID;
                        }
                    }
                    else if constexpr (std::is_same_v<std::decay_t<decltype(obj)>, std::string>)
                    {
                        if (!m_SceneByName.contains(obj))
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

    void Project::InitializeStart()
    {
        const SceneSpecification& startScene = std::visit(
            [&](auto&& obj) -> const SceneSpecification&
            {
                if constexpr (std::is_same_v<std::decay_t<decltype(obj)>, uint64_t>)
                {
                    return *m_SceneByUUID[obj];
                }
                else if constexpr (std::is_same_v<std::decay_t<decltype(obj)>, std::string>)
                {
                    return *m_SceneByName[obj];
                }
            },
            m_Specification.StartScene
        );

        Logger::Info("[Project] Initializing start scene: \"{0}\" with id: {1}", startScene.Name, startScene.UUID);

        SceneTable table = startScene.LoadSceneFn();
        Scene scene = Scene(startScene, std::move(table));
    }

}