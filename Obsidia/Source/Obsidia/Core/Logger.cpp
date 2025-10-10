#include "obpch.h"
#include "Logger.hpp"

#include "Obsidia/Core/Core.hpp"

#include <iostream>
#include <fstream>
#include <algorithm>
#include <ranges>

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
    // Constructor & Destructor
    ////////////////////////////////////////////////////////////////////////////////////
    FileSink::FileSink(const std::filesystem::path& file, const Sink::Level filter)
        : m_File(file), m_Filter(filter)
    {
    }

    ////////////////////////////////////////////////////////////////////////////////////
    // Methods
    ////////////////////////////////////////////////////////////////////////////////////
    void FileSink::Output(const Level level, const std::string_view message) const
    {
        if (static_cast<bool>(m_Filter & level))
        {
            // Note: Opens in append mode instead of overwrite
            std::ofstream file(m_File, std::ios::app); // Gets closed at the end of the scope
            
            if (!file.is_open() && file.good())
            {
                Logger::Error("Failed to log message: \"\" with level {} to file \"{}\"", message, Sink::LevelToTag(level), m_File.string());
                return;
            }

            file << message;
        }
    }

    ////////////////////////////////////////////////////////////////////////////////////
    // Static helpers
    ////////////////////////////////////////////////////////////////////////////////////
    void Logger::RemoveSink(mut Sink* sink)
    {
        const auto position = std::ranges::find(s_Sinks, std::shared_ptr<Sink>(sink));

        if (position != s_Sinks.end())
            s_Sinks.erase(position);
    }

    ////////////////////////////////////////////////////////////////////////////////////
    // Static methods
    ////////////////////////////////////////////////////////////////////////////////////
    void Logger::Output(const Sink::Level level, const std::string& message)
    {
        for (const std::shared_ptr<Sink> sink : s_Sinks)
            sink->Output(level, std::format("[{0}] [{1}]: {2}\n", ::Nano::Time::String(), Sink::LevelToTag(level), message));
    }

}