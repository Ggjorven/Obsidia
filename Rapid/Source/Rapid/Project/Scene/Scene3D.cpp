#include "rppch.h"
#include "Scene3D.hpp"

#include "Rapid/Core/Core.hpp"
#include "Rapid/Core/Logger.hpp"

namespace Rapid::Project
{

    ////////////////////////////////////////////////////////////////////////////////////
    // Constructor & Destructor
    ////////////////////////////////////////////////////////////////////////////////////
    Scene3D::Scene3D(const SceneSpecification& specs, Scene3DTable&& table)
        : m_Specification(specs), m_Table(std::move(table))
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

    void Scene3D::OnRender(Scene3DRenderer& renderer) const
    {
    }

    void Scene3D::OnEvent(const Event& e)
    {
    }

}