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

namespace Ob::Project
{

    ////////////////////////////////////////////////////////////////////////////////////
    // ProjectSpecification
    ////////////////////////////////////////////////////////////////////////////////////
    struct ProjectSpecification
    {
    public:
        std::string Name = {};

        std::vector<SceneSpecification> Scenes = {};
        std::variant<uint64_t, std::string> StartScene = 0ull;

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
        // Constructor & Destructor
        Project(const ProjectSpecification& specs);
        ~Project();

        // Methods
        void OnUpdate(float deltaTime);
        void OnRender();
        void OnEvent(const Obsidian::Event& e);

        // Getters
        inline const ProjectSpecification& GetSpecification() { return m_Specification; }

    private:
        enum class LoadSceneError : uint8_t { UUIDNotFound = 0, NameNotFound, NoLoadFunction };

    private:
        // Private methods
        void ProcessScenes();
        std::expected<std::shared_ptr<Scene>, LoadSceneError> LoadScene(const std::variant<uint64_t, std::string>& sceneIdentifier);

    private:
        ProjectSpecification m_Specification;

        std::unordered_map<uint64_t, SceneSpecification*> m_SceneSpecByUUID = {};
        std::unordered_map<std::string, SceneSpecification*> m_SceneSpecByName = {};

        std::unordered_map<uint64_t, std::shared_ptr<Scene>> m_SceneByUUID = {};
        std::unordered_map<std::string, std::shared_ptr<Scene>> m_SceneByName = {};
    };

}