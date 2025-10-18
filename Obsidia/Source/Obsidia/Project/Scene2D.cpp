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
    Scene2D::Scene2D(const SceneSpecification& specs, Scene2DTable&& table)
        : Scene(specs, table), m_Table(std::move(table))
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

    void Scene2D::OnRender()
    {
        for (auto& visualLayer : m_VisualLayers)
        {
            Renderer::BeginLayer(visualLayer);

            // Note: Render objects

            Renderer::EndLayer(visualLayer);
        }
    }

    void Scene2D::OnEvent(const Obsidian::Event& e)
    {
        //Nano::Events::EventHandler
    }

}