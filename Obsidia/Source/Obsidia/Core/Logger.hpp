#pragma once

#include "Obsidia/Core/Core.hpp"

#include <vector>
#include <format>
#include <string>
#include <memory>
#include <string_view>

namespace Ob
{

    ////////////////////////////////////////////////////////////////////////////////////
    // Sink
    ////////////////////////////////////////////////////////////////////////////////////
    class Sink
    {
    public:
        // Specifier structs
        enum class Level : uint8_t
        {
            Trace = 1 << 0, Info = 1 << 1, Warning = 1 << 2, Error = 1 << 3, Fatal = 1 << 4
        };

        // Static methods
        constexpr static std::string_view LevelToColour(const Level level)
        {
            switch (level)
            {
            case Level::Trace:      return ::Nano::Log::Colour::WhiteFG;
            case Level::Info:       return ::Nano::Log::Colour::GreenFG;
            case Level::Warning:    return ::Nano::Log::Colour::BrightYellowFG;
            case Level::Error:      return ::Nano::Log::Colour::BrightRedFG;
            case Level::Fatal:      return ::Nano::Log::Colour::RedBG;

            default:
                break;
            }

            return "<INVALID COLOUR>";
        }

        constexpr static std::string_view LevelToTag(const Level level)
        {
            switch (level)
            {
            case Level::Trace:      return "TRACE";
            case Level::Info:       return "INFO";
            case Level::Warning:    return "WARN";
            case Level::Error:      return "ERROR";
            case Level::Fatal:      return "FATAL";

            default:
                break;
            }

            return "<INVALID COLOUR>";
        }

    public:
        // Virtual methods
        virtual void Output(const Level level, const std::string_view message) const = 0;
    };
    NANO_DEFINE_BITWISE(Sink::Level)

    ////////////////////////////////////////////////////////////////////////////////////
    // Sinks
    ////////////////////////////////////////////////////////////////////////////////////
    class ConsoleSink : public Sink
    {
    public:
        // Constructor & Destructor
        ConsoleSink(const Sink::Level filter = Sink::Level::Trace | Sink::Level::Info | Sink::Level::Warning | Sink::Level::Error | Sink::Level::Fatal);
        virtual ~ConsoleSink() = default;

        // Methods
        virtual void Output(const Level level, const std::string_view message) const override;

    private:
        const Sink::Level m_Filter;
    };

    class FileSink : public Sink
    {
    public:
        // Constructor & Destructor

    };

    ////////////////////////////////////////////////////////////////////////////////////
    // Logger
    ////////////////////////////////////////////////////////////////////////////////////
    class Logger
    {
    public:
        // Static methods
        template<typename ...TArgs> static void Trace(std::format_string<TArgs...> fmt, TArgs&& ...args) { Output(Sink::Level::Trace, std::format(fmt, std::forward<TArgs>(args)...)); }
        template<typename ...TArgs> static void Info(std::format_string<TArgs...> fmt, TArgs&& ...args) { Output(Sink::Level::Info, std::format(fmt, std::forward<TArgs>(args)...)); }
        template<typename ...TArgs> static void Warning(std::format_string<TArgs...> fmt, TArgs&& ...args) { Output(Sink::Level::Warning, std::format(fmt, std::forward<TArgs>(args)...)); }
        template<typename ...TArgs> static void Error(std::format_string<TArgs...> fmt, TArgs&& ...args) { Output(Sink::Level::Error, std::format(fmt, std::forward<TArgs>(args)...)); }
        template<typename ...TArgs> static void Fatal(std::format_string<TArgs...> fmt, TArgs&& ...args) { Output(Sink::Level::Fatal, std::format(fmt, std::forward<TArgs>(args)...)); }

        // Static adder
        inline static void AddSink(mut std::shared_ptr<Sink> sink) { s_Sinks.emplace_back(sink); }

    private:
        // Private static methods
        static void Output(const Sink::Level level, const std::string& message);

    private:
        static mut std::vector<mut std::shared_ptr<Sink>> s_Sinks;
    };

}