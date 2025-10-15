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
    Scene::Scene(const SceneSpecification& specs, SceneTable&& table)
        : m_Specification(specs), m_Table(std::move(table))
    {
        Logger::Info("[Scene] Initializing scene named: \"{0}\"", m_Specification.Name);

        CreateVisualLayers();
    }

    Scene::~Scene()
    {
    }

    ////////////////////////////////////////////////////////////////////////////////////
    // Methods
    ////////////////////////////////////////////////////////////////////////////////////
    void Scene::OnUpdate(float deltaTime)
    {
    }

    void Scene::OnRender()
    {
    }

    void Scene::OnEvent(const Obsidian::Event& e)
    {
    }

    ////////////////////////////////////////////////////////////////////////////////////
    // Private methods
    ////////////////////////////////////////////////////////////////////////////////////
    void Scene::CreateVisualLayers()
    {
        Logger::Info("[Scene] Creating {0} VisualLayers from specification.", m_Table.GetVisualLayerSpecifications().size());

        // Sorting
        {
            // Note: Sorts based on Level
            std::ranges::sort(m_Table.GetVisualLayerSpecifications(), {}, &VisualLayerSpecification::Level);

            uint8_t level = 1;
            for (auto& layerSpec : m_Table.GetVisualLayerSpecifications())
            {
                if (layerSpec.Level != level)
                {
                    if (layerSpec.Level != 0) // Note: 0 means auto initialize
                        Logger::Warning("[Scene] VisualLayer's level doesn't match in hierarchy. Requested level is {0}, should be {1}.", layerSpec.Level, level);
                    layerSpec.Level = level;
                }

                level++;
            }

            if (m_Table.GetVisualLayerSpecifications().empty())
            {
                Logger::Warning("[Scene] No VisualLayer specification passed in. Creating a layer with level 1.");
                m_Table.GetVisualLayerSpecifications().emplace_back(VisualLayerSpecification().SetLevel(1));
            }
        }

        // Creation
        {
            m_VisualLayers.reserve(m_Table.GetVisualLayerSpecifications().size());
            for (const auto& layerSpec : m_Table.GetVisualLayerSpecifications())
                m_VisualLayers.emplace_back(layerSpec);
        }
    }

}