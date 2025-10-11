#pragma once

#include "Obsidia/Core/Core.hpp"

#include <Obsidian/Core/Events.hpp>

#include <cstdint>

namespace Ob::Project
{

    ////////////////////////////////////////////////////////////////////////////////////
    // ProjectSpecification
    ////////////////////////////////////////////////////////////////////////////////////
    struct ProjectSpecification
    {
    public:


    public:
        // Setters

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
        ProjectSpecification m_Specification;
    };

}