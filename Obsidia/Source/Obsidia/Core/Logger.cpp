#include "obpch.h"
#include "Logger.hpp"

#include "Obsidia/Core/Core.hpp"

#include <iostream>

namespace Ob
{

    ////////////////////////////////////////////////////////////////////////////////////
    // Static initialization
    ////////////////////////////////////////////////////////////////////////////////////
    mut std::vector<std::shared_ptr<Sink>> Logger::s_Sinks = []() -> std::vector<std::shared_ptr<Sink>>
        { 
            mut std::vector<std::shared_ptr<Sink>> sinks;
            sinks.reserve(2); 

#if !defined(OBSIDIA_CONFIG_DISTRIBUTION)
            sinks.emplace_back(new ConsoleSink());
#endif

            return sinks; 
        }();

    ////////////////////////////////////////////////////////////////////////////////////
    // Constructor & Destructor
    ////////////////////////////////////////////////////////////////////////////////////
    ConsoleSink::ConsoleSink(const Level filter)
        : m_Filter(filter)
    {
    }

    ////////////////////////////////////////////////////////////////////////////////////
    // Methods
    ////////////////////////////////////////////////////////////////////////////////////
    void ConsoleSink::Output(const Level level, const std::string_view message) const
    {
        if (static_cast<bool>(m_Filter & level))
            std::cout << Sink::LevelToColour(level) << message << ::Nano::Log::Colour::Reset;
    }

    ////////////////////////////////////////////////////////////////////////////////////
    // Static methods
    ////////////////////////////////////////////////////////////////////////////////////
    void Logger::Output(const Sink::Level level, const std::string& message)
    {
        for (const auto sink : s_Sinks)
            sink->Output(level, std::format("[{0}] [{1}]: {2}\n", ::Nano::Time::String(), Sink::LevelToTag(level), message));
    }

}