#pragma once

#include "Rapid/Core/Core.hpp"

#include "Rapid/Project/Scene.hpp"
#include "Rapid/Project/Events.hpp"

#include <cstdint>

namespace Rapid::Project
{

    ////////////////////////////////////////////////////////////////////////////////////
    // Scene3D
    ////////////////////////////////////////////////////////////////////////////////////
    class Scene3D : public Scene
    {
    public:
        // Constructor & Destructor
        Scene3D(Renderer& targetRenderer, const SceneSpecification& specs, Scene3DTable&& table);
        ~Scene3D();

        // Methods
        void OnUpdate(float deltaTime) override;
        void OnRender(Renderer& renderer) override;
        void OnEvent(const Event& e) override;

    private:
        Scene3DTable m_Table;
    };

}