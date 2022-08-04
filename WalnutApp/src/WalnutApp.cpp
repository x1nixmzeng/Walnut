#include "Walnut/Application.h"
#include "Walnut/EntryPoint.h"

#include "Walnut/Image.h"

class ExampleLayer : public Walnut::Layer
{
public:
	virtual void OnUIRender() override
	{
		ImGui::Begin("Hello");
		ImGui::Button("Button");
		ImGui::End();

		ImGui::ShowDemoWindow();
	}
};

Walnut::Application* Walnut::CreateApplication(int argc, char** argv)
{
	Walnut::ApplicationSpecification spec;
	spec.Name = "Walnut Example";

	Walnut::Application* app = new Walnut::Application(spec);

	auto example = std::make_shared<ExampleLayer>();
	app->PushLayer(example);

	app->SetMenubarCallback([app, example]()
	{
		if (ImGui::BeginMenu("File"))
		{
			if (app->ContainsLayer(example))
			{
				if (ImGui::MenuItem("Remove Example Layer"))
				{
					app->PopLayer(example);
				}
			}
			else
			{
				if (ImGui::MenuItem("Add Example Layer"))
				{
					app->PushLayer(example);
				}
			}

			if (ImGui::MenuItem("Exit"))
			{
				app->Close();
			}
			ImGui::EndMenu();
		}
	});
	return app;
}