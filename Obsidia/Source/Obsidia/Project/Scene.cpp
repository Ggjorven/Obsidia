#include "obpch.h"
#include "Scene.hpp"

#include "Obsidia/Core/Core.hpp"
#include "Obsidia/Core/Logger.hpp"

#include <ranges>

namespace Ob::Project
{

    ////////////////////////////////////////////////////////////////////////////////////
    // Constructor & Destructor
    ////////////////////////////////////////////////////////////////////////////////////
    Scene::Scene(const SceneSpecification& specs, const SceneTable& table)
        : m_Specification(specs), m_GlobalTable(table)
    {
        Logger::Info("[Scene] Initializing scene named: \"{0}\"", m_Specification.Name);

        CreateVisualLayers();
    }

    Scene::~Scene()
    {
    }

    ////////////////////////////////////////////////////////////////////////////////////
    // Private methods
    ////////////////////////////////////////////////////////////////////////////////////
    void Scene::CreateVisualLayers()
    {
        Logger::Info("[Scene] Creating {0} VisualLayers from specification.", m_GlobalTable.VisualLayers.size());

        // Sorting
        {
            // Note: Sorts based on Level
            std::ranges::sort(m_GlobalTable.VisualLayers, {}, &VisualLayerSpecification::Level);

            uint8_t level = 1;
            for (auto& layerSpec : m_GlobalTable.VisualLayers)
            {
                if (layerSpec.Level != level)
                {
                    if (layerSpec.Level != 0) // Note: 0 means auto initialize
                        Logger::Warning("[Scene] VisualLayer's level doesn't match in hierarchy. Requested level is {0}, should be {1}.", layerSpec.Level, level);
                    layerSpec.Level = level;
                }

                level++;
            }

            if (m_GlobalTable.VisualLayers.empty())
            {
                Logger::Warning("[Scene] No VisualLayer specification passed in. Creating a layer with level 1.");
                m_GlobalTable.VisualLayers.emplace_back(VisualLayerSpecification().SetLevel(1));
            }
        }

        // Creation
        {
            m_VisualLayers.reserve(m_GlobalTable.VisualLayers.size());
            for (const auto& layerSpec : m_GlobalTable.VisualLayers)
                m_VisualLayers.emplace_back(layerSpec);
        }
    }

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
    // Methods
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
    // Methods
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