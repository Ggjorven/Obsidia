#pragma once

#include "Obsidia/Core/Core.hpp"

#include "Obsidia/Project/Scene.hpp"

#include <Obsidian/Core/Events.hpp>

#include <cstdint>
#include <string>
#include <vector>
#include <variant>
#include <expected>
#include <unordered_map>

namespace Ob
{
    class Application;
}

namespace Ob::Project
{

    ////////////////////////////////////////////////////////////////////////////////////
    // SceneID
    ////////////////////////////////////////////////////////////////////////////////////
    using SceneID = std::variant<uint64_t, std::string>;

    ////////////////////////////////////////////////////////////////////////////////////
    // ProjectSpecification
    ////////////////////////////////////////////////////////////////////////////////////
    struct ProjectSpecification
    {
    public:
        std::string Name = {};

        std::vector<SceneSpecification> Scenes = {};
        SceneID StartScene = 0ull;

    public:
        // Setters
        inline ProjectSpecification& SetName(const std::string& name) { Name = name; return *this; }

        inline ProjectSpecification& SetScenes(std::vector<SceneSpecification>&& layers) { Scenes = std::move(layers); return *this; }
        inline ProjectSpecification& AddScene(const SceneSpecification& specs) { Scenes.emplace_back(specs); return *this; }
        inline ProjectSpecification& SetStartScene(uint64_t uuid) { StartScene = uuid; return *this; }
        inline ProjectSpecification& SetStartScene(const std::string& name) { StartScene = name; return *this; }
    };

    ////////////////////////////////////////////////////////////////////////////////////
    // Project
    ////////////////////////////////////////////////////////////////////////////////////
    class Project
    {
    public:
        enum class LoadSceneError : uint8_t { UUIDNotFound = 0, NameNotFound, NoLoadFunction };
        enum class UnloadSceneError : uint8_t { UUIDToSpecificationNotFound = 0, NameToSpecificationNotFound, UUIDToSceneNotFound, NameToSceneNotFound, SceneNotFound };
    public:
        // Constructor & Destructor
        Project(const ProjectSpecification& specs);
        ~Project();

        // Methods
        std::expected<std::shared_ptr<Scene>, LoadSceneError> LoadScene(const SceneID& sceneIdentifier);
        std::expected<void, UnloadSceneError> UnloadScene(const SceneID& sceneIdentifier);

        // Getters
        inline const ProjectSpecification& GetSpecification() { return m_Specification; }

    private:
        // Private methods
        void OnUpdate(float deltaTime);
        void OnRender();
        void OnEvent(const Obsidian::Event& e);

    private:
        // Private methods
        void ProcessScenes();

    private:
        ProjectSpecification m_Specification;

        std::unordered_map<uint64_t, SceneSpecification*> m_SceneSpecByUUID = {};
        std::unordered_map<std::string, SceneSpecification*> m_SceneSpecByName = {};

        std::unordered_map<uint64_t, std::shared_ptr<Scene>> m_SceneByUUID = {};
        std::unordered_map<std::string, std::shared_ptr<Scene>> m_SceneByName = {};

        friend class Application;
    };

}