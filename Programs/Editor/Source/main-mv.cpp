#include <Rapid/Core/Window.hpp>

#include <Rapid/Project/Renderer/Renderer.hpp>

using namespace Obsidian;

int main(const int argc, const char* argv[])
{
	(void)argc; (void)argv;

	std::queue<DeviceDestroyFn> destroyQueue;

	Window w = Window(WindowSpecification()
		.SetTitle("Rapid Application - TESTPROJECT")
		.SetWidthAndHeight(1280, 720)

		.SetEventCallback([&](const Obsidian::Event& e)
		{
			Nano::Events::EventHandler handler(e);
			handler.Handle<Obsidian::WindowResizeEvent>([&](const Obsidian::WindowResizeEvent& wre)
			{
			});

			// TODO: Pass through to project properly
		})
	);

	// BASE
	// Device
	Device device(DeviceSpecification()
		.SetNativeWindow(w.GetNativeWindow())
		.SetMessageCallback([&](DeviceMessageType type, std::string message) { std::cout << message << std::endl; })
		.SetDestroyCallback([&](DeviceDestroyFn fn) { destroyQueue.push(fn); })
	);

	// Swapchain
	Swapchain swapchain(device, SwapchainSpecification()
		.SetWindow(w)
		.SetFormat(Format::BGRA8Unorm)
		.SetColourSpace(ColourSpace::SRGB)
		.SetVSync(false)

		.SetDebugName("Swapchain")
	);

	std::array<Nano::Memory::DeferredConstruct<CommandListPool>, Information::FramesInFlight> commandPools = { };
	for (auto& pool : commandPools)
		pool.Construct(swapchain, CommandListPoolSpecification()
			.SetQueue(CommandQueue::Graphics)
			.SetDebugName("CommandPool")
		);

	// MAIN
	Obsidian::Renderpass pass0(device, Obsidian::RenderpassSpecification()
		.SetBindpoint(Obsidian::PipelineBindpoint::Graphics)

		.SetColourImageSpecification(swapchain.GetImage(0).GetSpecification())
		.SetColourLoadOperation(Obsidian::LoadOperation::Clear)
		.SetColourStoreOperation(Obsidian::StoreOperation::Store)
		.SetColourStartState(Obsidian::ResourceState::Present) // TODO: Change to something
		.SetColourRenderingState(Obsidian::ResourceState::RenderTarget)
		.SetColourEndState(Obsidian::ResourceState::RenderTarget)

		.SetDebugName("Scene2DPass")
	);

	// Create CommandLists for each frame
	std::array<Nano::Memory::DeferredConstruct<CommandList>, Information::FramesInFlight> commandLists0 = { };
	for (uint8_t i = 0; i < commandLists0.size(); i++)
		commandLists0[i].Construct(commandPools[i].Get(), Obsidian::CommandListSpecification()
			.SetDebugName(std::format("CommandList({0}) for Scene2DPass", i))
		);

	// Create framebuffers for renderpass
	for (uint8_t i = 0; i < Obsidian::Information::FramesInFlight; i++)
		pass0.CreateFramebuffer(Obsidian::FramebufferSpecification()
			.SetColourAttachment(Obsidian::FramebufferAttachment()
				.SetImage(swapchain.GetImage(i))
			));

	Obsidian::Renderpass pass1(device, Obsidian::RenderpassSpecification()
		.SetBindpoint(Obsidian::PipelineBindpoint::Graphics)

		.SetColourImageSpecification(swapchain.GetImage(0).GetSpecification())
		.SetColourLoadOperation(Obsidian::LoadOperation::Load)
		.SetColourStoreOperation(Obsidian::StoreOperation::Store)
		.SetColourStartState(Obsidian::ResourceState::RenderTarget)
		.SetColourRenderingState(Obsidian::ResourceState::RenderTarget)
		.SetColourEndState(Obsidian::ResourceState::Present)

		.SetDebugName("Scene2DPass")
	);

	// Create CommandLists for each frame
	std::array<Nano::Memory::DeferredConstruct<CommandList>, Information::FramesInFlight> commandLists1 = { };
	for (uint8_t i = 0; i < commandLists1.size(); i++)
		commandLists1[i].Construct(commandPools[i].Get(), Obsidian::CommandListSpecification()
			.SetDebugName(std::format("CommandList({0}) for Scene2DPass", i))
		);

	// Create framebuffers for renderpass
	for (uint8_t i = 0; i < Obsidian::Information::FramesInFlight; i++)
		pass1.CreateFramebuffer(Obsidian::FramebufferSpecification()
			.SetColourAttachment(Obsidian::FramebufferAttachment()
				.SetImage(swapchain.GetImage(i))
			));

	while (w.IsOpen())
	{
		w.PollEvents();

		{
			{
				while (!destroyQueue.empty())
				{
					destroyQueue.front()();
					destroyQueue.pop();
				}
			}

			swapchain.AcquireNextImage();
			commandPools[swapchain.GetCurrentFrame()]->Reset();
			
			auto& list0 = commandLists0[swapchain.GetCurrentFrame()].Get();
			auto& list1 = commandLists1[swapchain.GetCurrentFrame()].Get();

			{

				list0.Open();

				list0.StartRenderpass(Obsidian::RenderpassStartArgs()
					.SetRenderpass(pass0)

					.SetViewport(Obsidian::Viewport(static_cast<float>(w.GetSize().x), static_cast<float>(w.GetSize().y)))
					.SetScissor(Obsidian::ScissorRect(Obsidian::Viewport(static_cast<float>(w.GetSize().x), static_cast<float>(w.GetSize().y))))

					.SetColourClear({ 0.0f, 0.0f, 0.0f, 1.0f })
				);

				list0.EndRenderpass(Obsidian::RenderpassEndArgs()
					.SetRenderpass(pass0)
				);

				list0.Close();
				list0.Submit(Obsidian::CommandListSubmitArgs()
					.SetWaitForSwapchainImage(true)
					.SetOnFinishMakeSwapchainPresentable(false)
				);
			}

			{

				list1.Open();

				list1.StartRenderpass(Obsidian::RenderpassStartArgs()
					.SetRenderpass(pass1)

					.SetViewport(Obsidian::Viewport(static_cast<float>(w.GetSize().x), static_cast<float>(w.GetSize().y)))
					.SetScissor(Obsidian::ScissorRect(Obsidian::Viewport(static_cast<float>(w.GetSize().x), static_cast<float>(w.GetSize().y))))
				);

				list1.EndRenderpass(Obsidian::RenderpassEndArgs()
					.SetRenderpass(pass1)
				);

				list1.Close();
				list1.Submit(Obsidian::CommandListSubmitArgs()
					.SetWaitOnLists({ &list0 })
					.SetWaitForSwapchainImage(false)
					.SetOnFinishMakeSwapchainPresentable(true)
				);
			}

			swapchain.Present();
		}

		w.SwapBuffers();
	}

	return 0;
}