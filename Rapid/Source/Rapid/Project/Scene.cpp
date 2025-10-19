#include "rppch.h"
#include "Scene.hpp"

#include "Rapid/Core/Core.hpp"
#include "Rapid/Core/Logger.hpp"

#include <ranges>

namespace Rapid::Project
{

    ////////////////////////////////////////////////////////////////////////////////////
    // Constructor & Destructor
    ////////////////////////////////////////////////////////////////////////////////////
    Scene::Scene(Renderer& renderer, const SceneSpecification& specs, const SceneTable& table)
        : m_TargetRenderer(renderer), m_Specification(specs), m_GlobalTable(table)
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
                m_VisualLayers.emplace_back(m_TargetRenderer, layerSpec);
        }
    }

}