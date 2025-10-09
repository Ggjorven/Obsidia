#include <Obsidia/Core/Core.hpp>
#include <Obsidia/Core/Logger.hpp>

using namespace Ob;

int main(const int argc, const char* argv[])
{
	(void)argc; (void)argv;

	Logger::Trace("Printing: {0}", 10);
	Logger::Info("Printing: {0}", 10);
	Logger::Warning("Printing: {0}", 10);
	Logger::Error("Printing: {0}", 10);
	Logger::Fatal("Printing: {0}", 10);
	return 0;
}