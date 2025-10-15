#pragma once

#include "Obsidia/Core/Core.hpp"

#include "Obsidia/ECS/Components2D.hpp"
#include "Obsidia/ECS/Components3D.hpp"

#include <cstdint>
#include <tuple>

namespace Ob::ECS
{

    ////////////////////////////////////////////////////////////////////////////////////
    // Registries
    ////////////////////////////////////////////////////////////////////////////////////
    using Registry2D = Nano::ECS::Registry<uint64_t, Components2D>;
    using Registry3D = Nano::ECS::Registry<uint64_t, Components3D>;

}