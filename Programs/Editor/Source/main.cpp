#include <Rapid/Core/Window.hpp>

using namespace Rapid;

int main(const int argc, const char* argv[])
{
	(void)argc; (void)argv;

	Window w = Window(WindowSpecification()
		.SetTitle("Obsidia Application - TESTPROJECT")
		.SetWidthAndHeight(1280, 720)
		
		.SetEventCallback([](const Obsidian::Event& e)
		{
			// TODO: Pass through to project
		})
	);

	Project::Renderer renderer(w, 1280, 720);
	Project::Project project(renderer, Project::ProjectSpecification()
		.SetName("TESTPROJECT")
		.AddScene(Project::SceneSpecification()
			.SetName("TESTSCENE")
			.SetUUID(69420)

			.SetLoadSceneFn([](const Project::SceneSpecification& specs) -> Project::Scene2DTable
			{
				Project::Scene2DTable table;
				return table;
			})
		)

		.SetStartScene(69420)
	);

	float lastTime = w.GetTime();
	while (w.IsOpen())
	{
		// Update
		float currentTime = w.GetTime();
		float deltaTime = currentTime - lastTime;
		project.OnUpdate(deltaTime);
		lastTime = currentTime;

		// Render
		project.OnRender();

		w.SwapBuffers();
		w.PollEvents();
	}

	return 0;
}