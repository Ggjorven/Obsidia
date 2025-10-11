#include <Obsidia/Core/Application.hpp>

using namespace Ob;

int main(const int argc, const char* argv[])
{
	(void)argc; (void)argv;

	Ob::Application app = Ob::Application(
		Ob::ApplicationSpecification()
			.SetTitle("AAAAA")
			.SetWidthAndHeight(1280, 720)

			//.SetProject()
	);
	app.Run();

	return 0;
}