#include "obpch.h"
#include "Scene3D.hpp"

#include "Obsidia/Core/Core.hpp"
#include "Obsidia/Core/Logger.hpp"

namespace Ob::Project
{

    ////////////////////////////////////////////////////////////////////////////////////
    // Constructor & Destructor
    ////////////////////////////////////////////////////////////////////////////////////
    Scene3D::Scene3D(const SceneSpecification& specs, Scene3DTable&& table)
        : Scene(specs, table), m_Table(std::move(table))
    {
    }

    Scene3D::~Scene3D()
    {
    }

    ////////////////////////////////////////////////////////////////////////////////////
    // Private methods
    ////////////////////////////////////////////////////////////////////////////////////
    void Scene3D::OnUpdate(float deltaTime)
    {
    }

    void Scene3D::OnRender()
    {
    }

    void Scene3D::OnEvent(const Obsidian::Event& e)
    {
    }

}