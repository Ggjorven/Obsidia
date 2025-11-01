#include <Rapid/Core/Window.hpp>

#include <Rapid/Project/Renderer/Renderer.hpp>

using namespace Rapid;

class TestApplication
{
public:
	// Constructor & Destructor
	TestApplication()
		: m_Window(WindowSpecification()
			.SetTitle("Rapid Application - TESTPROJECT")
			.SetWidthAndHeight(1280, 720)

			.SetEventCallback([this](const Obsidian::Event& e) { OnEvent(e); })
		)
		, m_Project(GetProjectSpecification())
		, m_Renderer(m_Window)
	{
	}
	~TestApplication() = default;

	// Methods
	void Run()
	{
		float lastTime = m_Window.GetTime();

		while (m_Window.IsOpen())
		{
			m_Window.PollEvents();

			// Update
			float currentTime = m_Window.GetTime();
			float deltaTime = currentTime - lastTime;
			m_Project.OnUpdate(deltaTime);
			lastTime = currentTime;

			// Render // Note: Renders directly to swapchain image
			m_Renderer.Render(m_Project);

			m_Window.SwapBuffers();
		}
	}

private:
	Project::ProjectSpecification GetProjectSpecification()
	{
		return Project::ProjectSpecification()
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
			.SetStartScene(69420);
	}

	void OnEvent(const Obsidian::Event& e)
	{
		Nano::Events::EventHandler handler(e);
		handler.Handle<Obsidian::WindowResizeEvent>([this](const Obsidian::WindowResizeEvent& wre)
		{
			m_Renderer.Resize(wre.GetWidth(), wre.GetHeight());
			m_Project.OnEvent(Project::ResizeEvent(wre));
		});
	}

private:
	Window m_Window;
	
	Project::Project m_Project;
	Project::Renderer m_Renderer;
};

int main(const int argc, const char* argv[])
{
	(void)argc; (void)argv;

	TestApplication app;
	app.Run();

	return 0;
}