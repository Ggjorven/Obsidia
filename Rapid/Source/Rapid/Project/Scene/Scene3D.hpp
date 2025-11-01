#pragma once

#include "Rapid/Core/Core.hpp"

#include "Rapid/Project/Events.hpp"
#include "Rapid/Project/Renderer/Renderer.hpp"

#include "Rapid/Project/Scene/SceneSpecification.hpp"

#include <cstdint>

namespace Rapid::Project
{

    ////////////////////////////////////////////////////////////////////////////////////
    // Scene3D
    ////////////////////////////////////////////////////////////////////////////////////
    class Scene3D
    {
    public:
        // Constructor & Destructor
        Scene3D(const SceneSpecification& specs, Scene3DTable&& table);
        ~Scene3D();

        // Methods
        void OnUpdate(float deltaTime);
        void OnRender(Scene3DRenderer& renderer) const;
        void OnEvent(const Event& e);

        inline const SceneSpecification& GetSpecification() const { return m_Specification; }

    private:
        SceneSpecification m_Specification;
        Scene3DTable m_Table;
    };
}