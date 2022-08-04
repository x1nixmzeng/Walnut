#pragma once

#include "Layer.h"

#include <string>
#include <vector>
#include <memory>
#include <functional>
#include <algorithm>

#include "imgui.h"
#include "vulkan/vulkan.h"

void check_vk_result(VkResult err);

struct GLFWwindow;

namespace Walnut {

	struct ApplicationSpecification
	{
		std::string Name = "Walnut App";
		uint32_t Width = 1600;
		uint32_t Height = 900;
	};

	class Application
	{
	public:
		Application(const ApplicationSpecification& applicationSpecification = ApplicationSpecification());
		~Application();

		void Run();
		void SetMenubarCallback(const std::function<void()>& menubarCallback) { m_MenubarCallback = menubarCallback; }
		
		template<typename T>
		bool ContainsLayer(const std::shared_ptr<T>& layer) const
		{
			static_assert(std::is_base_of<Layer, T>::value, "Layer type is not subclass of Layer!");
			return std::find(m_LayerStack.begin(), m_LayerStack.end(), layer) != m_LayerStack.end();
		}

		template<typename T>
		void PushLayer(const std::shared_ptr<T>& layer)
		{
			static_assert(std::is_base_of<Layer, T>::value, "Pushed type is not subclass of Layer!");
			m_LayerStack.emplace_back(layer)->OnAttach();
		}

		template<typename T>
		void PopLayer(const std::shared_ptr<T>& layer)
		{
			static_assert(std::is_base_of<Layer, T>::value, "Popped type is not subclass of Layer!");
			auto it = std::find(m_LayerStack.begin(), m_LayerStack.end(), layer);
			if (it != m_LayerStack.end())
			{
				m_LayerStack.erase(it);
			}
		}

		void Close();

		static VkInstance GetInstance();
		static VkPhysicalDevice GetPhysicalDevice();
		static VkDevice GetDevice();

		static VkCommandBuffer GetCommandBuffer(bool begin);
		static void FlushCommandBuffer(VkCommandBuffer commandBuffer);

		static void SubmitResourceFree(std::function<void()>&& func);
	private:
		void Init();
		void Shutdown();
	private:
		ApplicationSpecification m_Specification;
		GLFWwindow* m_WindowHandle = nullptr;
		bool m_Running = false;

		std::vector<std::shared_ptr<Layer>> m_LayerStack;
		std::function<void()> m_MenubarCallback;
	};

	// Implemented by CLIENT
	Application* CreateApplication(int argc, char** argv);
}