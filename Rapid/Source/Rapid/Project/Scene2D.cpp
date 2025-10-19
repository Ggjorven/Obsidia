#include "rppch.h"
#include "Scene2D.hpp"

#include "Rapid/Core/Core.hpp"
#include "Rapid/Core/Logger.hpp"

#include "Rapid/Renderer/Renderer.hpp"

namespace Rapid::Project
{

    ////////////////////////////////////////////////////////////////////////////////////
    // Constructor & Destructor
    ////////////////////////////////////////////////////////////////////////////////////
    Scene2D::Scene2D(Renderer& targetRenderer, const SceneSpecification& specs, Scene2DTable&& table)
        : Scene(targetRenderer, specs, table), m_Table(std::move(table))
    {
    }

    Scene2D::~Scene2D()
    {
    }

    ////////////////////////////////////////////////////////////////////////////////////
    // Private methods
    ////////////////////////////////////////////////////////////////////////////////////
    void Scene2D::OnUpdate(float deltaTime)
    {
    }

    void Scene2D::OnRender(Renderer& renderer)
    {
        for (auto& visualLayer : m_VisualLayers)
        {
            m_TargetRenderer.BeginLayer(visualLayer);

            // Note: Render objects

            m_TargetRenderer.EndLayer(visualLayer);
        }
    }

    void Scene2D::OnEvent(const Event& e)
    {
        Nano::Events::EventHandler handler(e);
        handler.Handle<ResizeEvent>([&](const ResizeEvent& re) 
        {
            (void)re;
            for (auto& visualLayer : m_VisualLayers)
                visualLayer.Resize();
        });
    }

}