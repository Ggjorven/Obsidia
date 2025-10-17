#pragma once

#include <Nano/Nano.hpp>

#include <cstdint>

namespace Ob
{

    ////////////////////////////////////////////////////////////////////////////////////
    // ErrorCode
    ////////////////////////////////////////////////////////////////////////////////////
    enum class ErrorCode : uint32_t
    {
        None = 0,

        UUIDToSpecificationNotFound, 
        NameToSpecificationNotFound, 
        NoLoadFunction,

        UUIDToSceneNotFound, 
        NameToSceneNotFound, 
        SceneNotFound, // Both UUID & Name not found
    };

}