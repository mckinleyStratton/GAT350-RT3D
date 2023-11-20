#include "Editor.h"
#include "Scene.h"

namespace nc 
{

	void Editor::ProgessGui(Scene* scene)
	{
		// show resources
		ImGui::Begin("Resources");

		auto resources = GET_RESOURCES(Resource);


		ImGui::End();

		scene->ProcessGui();
	};
}
