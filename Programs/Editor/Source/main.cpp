#include <Obsidia/Core/Application.hpp>

using namespace Ob;

int main(const int argc, const char* argv[])
{
	(void)argc; (void)argv;

	Application app = Application(ApplicationSpecification()
		.SetTitle("AAAAA")
		.SetWidthAndHeight(1280, 720)

		.SetProject(Project::ProjectSpecification()
			.AddScene(Project::SceneSpecification()
				.AddVisualLayer(Project::VisualLayerSpecification())
				.AddVisualLayer(Project::VisualLayerSpecification())
			)
		)
	);
	app.Run();

	return 0;
}