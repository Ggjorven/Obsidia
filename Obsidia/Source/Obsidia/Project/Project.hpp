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
    // SceneCollection
    ////////////////////////////////////////////////////////////////////////////////////
    struct SceneCollection
    {
    public:
        std::unordered_map<uint64_t, SceneSpecification> SceneSpecByUUID = {};
        std::unordered_map<std::string, SceneSpecification> SceneSpecByName = {};

        std::unordered_map<uint64_t, std::shared_ptr<Scene>> SceneByUUID = {};
        std::unordered_map<std::string, std::shared_ptr<Scene>> SceneByName = {};

    public:
        // Adders
        void AddSpecification(const SceneSpecification& specs);
        void AddScene(std::shared_ptr<Scene> scene);

        // Getters
        bool SpecContains(const SceneID& sceneIdentifier) const;
        bool SceneLoaded(const SceneID& sceneIdentifier) const;

        std::expected<SceneSpecification, ErrorCode> GetSpecification(const SceneID& sceneIdentifier);
    };

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
        // Constructor & Destructor
        Project(const ProjectSpecification& specs);
        ~Project();

        // Methods
        /*
         * @brief Loads scene into memory and holds a shared_ptr/reference to it.
         * @param SceneID, which is used to load the specification to be able to get the load function.
         * @return The Scene or an ErrorCode: UUIDToSpecificationNotFound, NameToSpecificationNotFound or NoLoadFunction
         */
        std::expected<std::shared_ptr<Scene>, ErrorCode> LoadScene(const SceneID& sceneIdentifier);

        /*
         * @brief Removes scene from internal maps referencing scene
         * @param SceneID, which is used to unload the proper scene.
         * @return Nothing or An ErrorCode: UUIDToSpecificationNotFound, NameToSpecificationNotFound, UUIDToSceneNotFound, NameToSceneNotFound or SceneNotFound
         */
        std::expected<void, ErrorCode> UnloadScene(const SceneID& sceneIdentifier);

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

        SceneCollection m_Scenes = {};

        friend class Application;
    };

}