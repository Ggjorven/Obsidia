#pragma once

#include "Obsidia/Core/Core.hpp"

#include "Obsidia/Project/Scene.hpp"
#include "Obsidia/Project/Events.hpp"

#include <cstdint>

namespace Ob::Project
{

    ////////////////////////////////////////////////////////////////////////////////////
    // Scene3D
    ////////////////////////////////////////////////////////////////////////////////////
    class Scene3D : public Scene
    {
    public:
        // Constructor & Destructor
        Scene3D(const SceneSpecification& specs, Scene3DTable&& table);
        ~Scene3D();

        // Methods
        void OnUpdate(float deltaTime) override;
        void OnRender() override;
        void OnEvent(const Event& e) override;

    private:
        Scene3DTable m_Table;
    };

}