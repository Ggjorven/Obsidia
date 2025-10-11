#include <Obsidia/Core/Core.hpp>
#include <Obsidia/Core/Logger.hpp>

#include <Obsidian/Core/Window.hpp>

using namespace Ob;

int main(const int argc, const char* argv[])
{
	(void)argc; (void)argv;

	Obsidian::Window window = Obsidian::Window(Obsidian::WindowSpecification()
		.SetTitle("Test")
		.SetWidthAndHeight(1280, 720)
		.SetEventCallback([](const Obsidian::Event e) { })
		.SetFlags(Obsidian::WindowFlags::Default)
	);

	while (window.IsOpen())
	{
		window.PollEvents();

		window.SwapBuffers();
	}

	return 0;
}