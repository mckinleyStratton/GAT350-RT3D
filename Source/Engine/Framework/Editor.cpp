#include "Editor.h"
#include "Scene.h"

namespace nc 
{
	void Editor::Update()
	{
		if (ImGui::IsKeyPressed(ImGuiKey_GraveAccent)) m_active = !m_active;
	}

	void Editor::ProgessGui(Scene* scene)
	{
		if (!m_active) return;

		// show resources
		ImGui::Begin("Resources");

		auto resources = GET_RESOURCES(Resource);
		for (auto& resource : resources)
		{
			if (ImGui::Selectable(resource->name.c_str(), resource.get() == m_selected))
			{
				m_selected = resource.get();
			}
		}

		ImGui::End();

		// show scene
		ImGui::Begin("Scene");
		scene->ProcessGui();
		ImGui::Separator();
		
		// show actors
		ImGui::BeginChild("Actors");
		// actor controls
		if (ImGui::BeginPopupContextWindow())
		{
			if (ImGui::MenuItem("Create Empty")) {}
			if (ImGui::MenuItem("Sphere")) {}
			if (ImGui::MenuItem("Cube")) {}
			if (ImGui::MenuItem("Camera")) {}
			if (ImGui::MenuItem("Light")) {}

			ImGui::EndPopup();
		}


		for (auto& actor : scene->m_actors)
		{
			if (ImGui::Selectable(actor->name.c_str(), actor.get() == m_selected))
			{
				m_selected = actor.get();
			}
		}

		
		ImGui::EndChild();
		ImGui::End();

		// show inspector
		ImGui::Begin("Inspector");
		if (m_selected)
		{
			m_selected->ProcessGui();
			// delete selected actor
			if (ImGui::IsKeyPressed(ImGuiKey_Delete))
			{
				auto actor = dynamic_cast<Actor*>(m_selected);
				if (actor)
				{
					scene->Remove(actor);
					m_selected = nullptr;
				}
			}
		}
		ImGui::End();


	};
}
