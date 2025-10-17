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

				.SetLoadSceneFn([](const Project::SceneSpecification& specs) -> Project::Scene2DTable
				{
					Logger::Info("Load scene function being called.");

					Project::Scene2DTable table = {};
					return table;
				})
			)
			
			.SetStartScene(10)
		)
	);
	app.Run();

	return 0;
}