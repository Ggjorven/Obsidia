#include "Obsidian/../../../Sandbox/src/Tests/TestBase.hpp"
#include "Obsidian/../../../Sandbox/src/Common/Camera2D.hpp"
#include "Obsidian/../../../Sandbox/src/Common/Camera2D.cpp"

#include <Obsidian/Maths/Functions.hpp>

////////////////////////////////////////////////////////////////////////////////////
// Test
////////////////////////////////////////////////////////////////////////////////////
class Renderpasses : public TestBase
{
public:
	// Constructor & Destructor
	Renderpasses()
		: TestBase(1280, 720, "Renderpasses", [this](Event e) { OnEvent(e); }, [this](DeviceMessageType type, const std::string& msg) { OnDeviceMessage(type, msg); })
	{
		// Commandpools & Commandlists
		for (auto& pool : m_CommandPools)
			pool.Construct(m_Swapchain.Get(), CommandListPoolSpecification()
				.SetQueue(CommandQueue::Graphics)
				.SetDebugName("CommandPool")
			);

		for (size_t i = 0; i < m_Pass0Lists.size(); i++)
		{
			m_Pass0Lists[i].Construct(m_CommandPools[i].Get(), CommandListSpecification()
				.SetDebugName(std::format("CommandList for: {0}", m_CommandPools[i]->GetSpecification().DebugName))
			);
		}

		for (size_t i = 0; i < m_Pass1Lists.size(); i++)
		{
			m_Pass1Lists[i].Construct(m_CommandPools[i].Get(), CommandListSpecification()
				.SetDebugName(std::format("CommandList for: {0}", m_CommandPools[i]->GetSpecification().DebugName))
			);
		}

		// Renderpasses
		m_Pass0.Construct(m_Device.Get(), RenderpassSpecification()
			.SetBindpoint(PipelineBindpoint::Graphics)

			.SetColourImageSpecification(m_Swapchain->GetImage(0).GetSpecification())
			.SetColourLoadOperation(LoadOperation::Clear)
			.SetColourStoreOperation(StoreOperation::Store)
			.SetColourStartState(ResourceState::Present)
			.SetColourRenderingState(ResourceState::RenderTarget)
			.SetColourEndState(ResourceState::RenderTarget)

			.SetDebugName("Pass0")
		);

		for (size_t i = 0; i < m_Swapchain->GetImageCount(); i++)
		{
			std::string debugName = std::format("Framebuffer({0}) for: {1}", i, m_Pass0->GetSpecification().DebugName);
			(void)m_Pass0->CreateFramebuffer(FramebufferSpecification()
				.SetColourAttachment(FramebufferAttachment()
					.SetImage(m_Swapchain->GetImage(static_cast<uint8_t>(i)))
				)
				.SetDebugName(debugName)
			);
		}

		m_Pass1.Construct(m_Device.Get(), RenderpassSpecification()
			.SetBindpoint(PipelineBindpoint::Graphics)

			.SetColourImageSpecification(m_Swapchain->GetImage(0).GetSpecification())
			.SetColourLoadOperation(LoadOperation::Load)
			.SetColourStoreOperation(StoreOperation::Store)
			.SetColourStartState(ResourceState::RenderTarget)
			.SetColourRenderingState(ResourceState::RenderTarget)
			.SetColourEndState(ResourceState::Present)

			.SetDebugName("Pass1")
		);

		for (size_t i = 0; i < m_Swapchain->GetImageCount(); i++)
		{
			std::string debugName = std::format("Framebuffer({0}) for: {1}", i, m_Pass1->GetSpecification().DebugName);
			(void)m_Pass1->CreateFramebuffer(FramebufferSpecification()
				.SetColourAttachment(FramebufferAttachment()
					.SetImage(m_Swapchain->GetImage(static_cast<uint8_t>(i)))
				)
				.SetDebugName(debugName)
			);
		}
	}

	~Renderpasses()
	{
		m_Device->DestroyRenderpass(m_Pass0.Get());
		m_Device->DestroyRenderpass(m_Pass1.Get());

		for (size_t i = 0; i < m_CommandPools.size(); i++)
		{
			m_CommandPools[i]->FreeList(m_Pass0Lists[i].Get());
			m_CommandPools[i]->FreeList(m_Pass1Lists[i].Get());
			m_Swapchain->FreePool(m_CommandPools[i].Get());
		}
	}

	// Methods
	void Run()
	{
		while (m_Window->IsOpen())
		{
			m_Window->PollEvents();
			FreeQueue();

			// Rendering
			{
				m_Swapchain->AcquireNextImage();
				{
					m_CommandPools[m_Swapchain->GetCurrentFrame()]->Reset();
					auto& list0 = m_Pass0Lists[m_Swapchain->GetCurrentFrame()];
					auto& list1 = m_Pass1Lists[m_Swapchain->GetCurrentFrame()];

					// First pass
					{
						list0->Open();

						list0->StartRenderpass(RenderpassStartArgs()
							.SetRenderpass(m_Pass0.Get())

							.SetViewport(Viewport(static_cast<float>(m_Window->GetSize().x), static_cast<float>(m_Window->GetSize().y)))
							.SetScissor(ScissorRect(Viewport(static_cast<float>(m_Window->GetSize().x), static_cast<float>(m_Window->GetSize().y))))

							.SetColourClear({ 0.0f, 0.0f, 0.0f, 1.0f })
						);

						list0->EndRenderpass(RenderpassEndArgs()
							.SetRenderpass(m_Pass0.Get())
						);

						list0->Close();
						list0->Submit(CommandListSubmitArgs()
							.SetWaitForSwapchainImage(true)
							.SetOnFinishMakeSwapchainPresentable(false)
						);
					}

					// Second pass
					{
						list1->Open();

						list1->StartRenderpass(RenderpassStartArgs()
							.SetRenderpass(m_Pass1.Get())

							.SetViewport(Viewport(static_cast<float>(m_Window->GetSize().x), static_cast<float>(m_Window->GetSize().y)))
							.SetScissor(ScissorRect(Viewport(static_cast<float>(m_Window->GetSize().x), static_cast<float>(m_Window->GetSize().y))))
						);

						list1->EndRenderpass(RenderpassEndArgs()
							.SetRenderpass(m_Pass1.Get())
						);

						list1->Close();
						list1->Submit(CommandListSubmitArgs()
							.SetWaitOnLists({ &list0.Get() })
							.SetWaitForSwapchainImage(false)
							.SetOnFinishMakeSwapchainPresentable(true)
						);
					}
				}
				m_Swapchain->Present();
			}
		}
	}

private:
	// Private methods
	void OnEvent(Event& e)
	{
		Nano::Events::EventHandler handler(e);
		handler.Handle<WindowCloseEvent>([&](WindowCloseEvent&) mutable { m_Window->Close(); });
		handler.Handle<WindowResizeEvent>([&](WindowResizeEvent& wre) mutable
		{
			m_Swapchain->Resize(wre.GetWidth(), wre.GetHeight());
			m_Pass0->ResizeFramebuffers();
			m_Pass1->ResizeFramebuffers();
		});
	}

	void OnDeviceMessage(DeviceMessageType msgType, const std::string& message)
	{
		switch (msgType)
		{
		case DeviceMessageType::Warn:
			OB_LOG_WARN("Device Warning: {0}", message);
			break;
		case DeviceMessageType::Error:
			OB_LOG_ERROR("Device Error: {0}", message);
			break;

		default:
			break;
		}
	}

	void FreeQueue()
	{
		while (!m_DestroyQueue.empty())
		{
			m_DestroyQueue.front()();
			m_DestroyQueue.pop();
		}
	}

private:
	std::array<Nano::Memory::DeferredConstruct<CommandListPool>, Information::FramesInFlight> m_CommandPools = { };
	std::array<Nano::Memory::DeferredConstruct<CommandList>, Information::FramesInFlight> m_Pass0Lists = { };
	std::array<Nano::Memory::DeferredConstruct<CommandList>, Information::FramesInFlight> m_Pass1Lists = { };

	Nano::Memory::DeferredConstruct<Renderpass> m_Pass0 = {};
	Nano::Memory::DeferredConstruct<Renderpass> m_Pass1 = {};

	std::queue<DeviceDestroyFn> m_DestroyQueue = {};
};

int Main(int argc, char* argv[])
{
	(void)argc; (void)argv;

	Renderpasses app;
	app.Run();
	return 0;
}

// Note: On windows to remove the terminal on distribution we need a special main function
// on linux and macos a regular main function is fine.
#if defined(OB_CONFIG_DIST) && defined(OB_PLATFORM_WINDOWS) 
#include <Windows.h>
int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ PSTR, _In_ int)
{
	return Main(__argc, __argv);
}
#else
int main(int argc, char* argv[])
{
	return Main(argc, argv);
}
#endif