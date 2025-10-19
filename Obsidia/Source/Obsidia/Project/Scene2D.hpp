#pragma once

#include "Obsidia/Core/Core.hpp"

#include "Obsidia/Project/Scene.hpp"
#include "Obsidia/Project/Events.hpp"
#include "Obsidia/Project/Renderer/Renderer.hpp"

#include <cstdint>

namespace Ob::Project
{

    ////////////////////////////////////////////////////////////////////////////////////
    // Scene2D
    ////////////////////////////////////////////////////////////////////////////////////
    class Scene2D : public Scene
    {
    public:
        // Constructor & Destructor
        Scene2D(Renderer& targetRenderer, const SceneSpecification& specs, Scene2DTable&& table);
        ~Scene2D();

        // Methods
        void OnUpdate(float deltaTime) override;
        void OnRender(Renderer& renderer) override;
        void OnEvent(const Event& e) override;

    private:
        Scene2DTable m_Table;
    };

}