#pragma once

#include "Obsidia/Core/Core.hpp"

#include "Obsidia/Project/Scene.hpp"

#include <Obsidian/Core/Events.hpp>

#include <cstdint>
#include <vector>

namespace Ob::Project
{

    ////////////////////////////////////////////////////////////////////////////////////
    // ProjectSpecification
    ////////////////////////////////////////////////////////////////////////////////////
    struct ProjectSpecification
    {
    public:
        std::vector<SceneSpecification> Scenes = {};

    public:
        // Setters
        inline ProjectSpecification& SetScenes(std::vector<SceneSpecification>&& layers) { Scenes = std::move(layers); return *this; }
        inline ProjectSpecification& AddScene(const SceneSpecification& specs) { Scenes.emplace_back(specs); return *this; }
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
        inline ProjectSpecification& GetSpecification() { return m_Specification; }

    private:
        // Private methods
        void ProcessScenes();

    private:
        ProjectSpecification m_Specification;

    };

}