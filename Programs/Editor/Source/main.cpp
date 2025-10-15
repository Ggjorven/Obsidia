#include <Obsidia/Core/Application.hpp>

using namespace Ob;

int main(const int argc, const char* argv[])
{
	(void)argc; (void)argv;

	Application app = Application(ApplicationSpecification()
		.SetTitle("Obsidia Application - TESTPROJECT")
		.SetWidthAndHeight(1280, 720)

		.SetProject(Project::ProjectSpecification()
			.SetName("TESTPROJECT")

			.AddScene(Project::SceneSpecification()
				.SetName("MainMenu")
				.SetUUID(10)

			)
			
			.SetStartScene(10)
		)
	);
	app.Run();

	return 0;
}