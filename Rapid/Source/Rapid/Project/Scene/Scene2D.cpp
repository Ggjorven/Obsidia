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
    Scene2D::Scene2D(const SceneSpecification& specs, Scene2DTable&& table)
        : m_Specification(specs), m_Table(std::move(table))
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

    void Scene2D::OnRender(Scene2DRenderer& renderer) const
    {
        
    }

    void Scene2D::OnEvent(const Event& e)
    {
        //Nano::Events::EventHandler handler(e);
       
    }

}