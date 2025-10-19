#include "obpch.h"
#include "Scene2D.hpp"

#include "Obsidia/Core/Core.hpp"
#include "Obsidia/Core/Logger.hpp"

#include "Obsidia/Renderer/Renderer.hpp"

namespace Ob::Project
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