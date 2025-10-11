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
    Scene::Scene(const SceneSpecification& specs)
        : m_Specification(specs)
    {
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
        // Sorting
        {
            // Note: Sorts based on Level
            std::ranges::sort(m_Specification.VisualLayers, {}, &VisualLayerSpecification::Level);

            uint8_t level = 1;
            for (auto& layerSpec : m_Specification.VisualLayers)
            {
                if (layerSpec.Level != level)
                {
                    if (layerSpec.Level != 0) // Note: 0 means auto initialize
                        Logger::Warning("VisualLayer's level doesn't match in hierarchy. Requested level is {}, should be {}.", layerSpec.Level, level);
                    layerSpec.Level = level;
                }

                level++;
            }
        }

        // Creation
        {
            m_VisualLayers.reserve(m_Specification.VisualLayers.size());
            for (const auto& layerSpec : m_Specification.VisualLayers)
                m_VisualLayers.emplace_back(layerSpec);
        }
    }

}