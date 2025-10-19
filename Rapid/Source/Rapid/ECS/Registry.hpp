#pragma once

#include "Rapid/Core/Core.hpp"

#include "Rapid/ECS/Components2D.hpp"
#include "Rapid/ECS/Components3D.hpp"

#include <cstdint>
#include <tuple>

namespace Rapid::ECS
{

    ////////////////////////////////////////////////////////////////////////////////////
    // Registries
    ////////////////////////////////////////////////////////////////////////////////////
    using Registry2D = Nano::ECS::Registry<uint64_t, Components2D>;
    using Registry3D = Nano::ECS::Registry<uint64_t, Components3D>;

}