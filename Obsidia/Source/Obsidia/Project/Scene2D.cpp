#include "obpch.h"
#include "Scene2D.hpp"

#include "Obsidia/Core/Core.hpp"
#include "Obsidia/Core/Logger.hpp"

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
    }

    void Scene2D::OnEvent(const Obsidian::Event& e)
    {
    }

}