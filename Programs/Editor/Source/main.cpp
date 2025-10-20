#include <Rapid/Core/Window.hpp>

#include <Rapid/Project/Renderer/Renderer.hpp>

using namespace Rapid;

int main(const int argc, const char* argv[])
{
	(void)argc; (void)argv;

	Project::Renderer* rendererPtr;
	Project::Project* projectPtr;

	Window w = Window(WindowSpecification()
		.SetTitle("Obsidia Application - TESTPROJECT")
		.SetWidthAndHeight(1280, 720)
		
		.SetEventCallback([&](const Obsidian::Event& e)
		{
				Nano::Events::EventHandler handler(e);
				handler.Handle<Obsidian::WindowResizeEvent>([&](const Obsidian::WindowResizeEvent& wre)
				{
					rendererPtr->Resize(wre.GetWidth(), wre.GetHeight());
					projectPtr->OnEvent(Project::ResizeEvent(wre));
				});

			// TODO: Pass through to project properly
		})
	);

	Project::Renderer renderer(w, 1280, 720);
	Project::Project project(Project::ProjectSpecification()
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
		project.OnRender(renderer);

		w.SwapBuffers();
		w.PollEvents();
	}

	return 0;
}