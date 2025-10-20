#pragma once

#include "Rapid/Core/Core.hpp"

#include "Rapid/Project/Events.hpp"
#include "Rapid/Project/Renderer/Renderer.hpp"

#include "Rapid/Project/Scene/SceneSpecification.hpp"

#include <cstdint>

namespace Rapid::Project
{

    ////////////////////////////////////////////////////////////////////////////////////
    // Scene2D
    ////////////////////////////////////////////////////////////////////////////////////
    class Scene2D
    {
    public:
        // Constructor & Destructor
        Scene2D(const SceneSpecification& specs, Scene2DTable&& table);
        ~Scene2D();

        // Methods
        void OnUpdate(float deltaTime);
        void OnRender(Renderer& renderer);
        void OnEvent(const Event& e);

        inline const SceneSpecification& GetSpecification() const { return m_Specification; }

    private:
        SceneSpecification m_Specification;
        Scene2DTable m_Table;
    };

}