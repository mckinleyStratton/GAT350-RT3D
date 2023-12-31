#include "World06.h"
#include "Framework/Framework.h"
#include "Input/InputSystem.h"

#include <glm/glm/gtc/type_ptr.hpp>
#include <glm/glm/gtx/color_space.hpp>


namespace nc
{
	bool World06::Initialize()
	{
		m_scene = std::make_unique<Scene>();
		m_scene->Load("Scenes/scene_framebuffer.json");
		m_scene->Initialize();

		auto texture = std::make_shared<Texture>();
		texture->CreateTexture(1024, 1024);
		ADD_RESOURCE("fb_texture", texture);

		auto framebuffer = std::make_shared<Framebuffer>();
		framebuffer->CreateFramebuffer(texture);
		ADD_RESOURCE("fb", framebuffer);

		auto material = GET_RESOURCE(Material, "materials/postprocess.mtrl");
		if (material)
		{
			material->albedoTexture = texture;

		}

		return true;
	};


	void World06::Shutdown()
	{
	}

	void World06::Update(float dt)
	{
		m_time += dt;
		
		ENGINE.GetSystem<Gui>()->BeginFrame();

		m_scene->Update(dt);
		m_scene->ProcessGui();

		// set postprocess gui
		ImGui::Begin("Post-Process");
		ImGui::SliderFloat("Blend", &m_blend, 0, 1);



		// GUI EFFECTS
//INVERT
		bool effect = m_params & INVERT_MASK;
		if (ImGui::Checkbox("Invert", &effect))
		{
			(effect) ? m_params |= INVERT_MASK : m_params &= ~INVERT_MASK;
		}

//GREYSCALE
		effect = m_params & GRAYSCALE_MASK;
		if (ImGui::Checkbox("Greyscale", &effect))
		{
			(effect) ? m_params |= GRAYSCALE_MASK : m_params &= ~GRAYSCALE_MASK; // changed &= ~
		}


//GRAIN
		effect = m_params & GRAIN_MASK;
		if (ImGui::Checkbox("Grain", &effect))
		{
			(effect) ? m_params |= GRAIN_MASK : m_params &= ~GRAIN_MASK;
		}


//SCANLINE
		effect = m_params & SCANLINE_MASK;
		if (ImGui::Checkbox("Scanline", &effect))
		{
			(effect) ? m_params |= SCANLINE_MASK : m_params &= ~SCANLINE_MASK;
		}


//BLURRY
		effect = m_params & BLURRY_MASK;
		if (ImGui::Checkbox("Blurry", &effect))
		{
			(effect) ? m_params |= BLURRY_MASK : m_params &= ~BLURRY_MASK;
		}


//COLOR TINT
		effect = m_params & COLORTINT_MASK;
		if (ImGui::Checkbox("Color Tint", &effect))
		{

			(effect) ? m_params |= COLORTINT_MASK : m_params &= ~COLORTINT_MASK;

		}
		if (m_params & COLORTINT_MASK)
		{
			ImGui::SliderFloat("RED Tint", &m_rTint, 0, 1);
			ImGui::SliderFloat("BLUE Tint", &m_bTint, 0, 1);
			ImGui::SliderFloat("GREEN Tint", &m_gTint, 0, 1);
		}

//ITS GIVING CYBERPUNK
		effect = m_params & CP_MASK;
		if (ImGui::Checkbox("Its Giving CyberPunk", &effect))
		{
			(effect) ? m_params |= CP_MASK : m_params &= ~CP_MASK;
		}

		ImGui::End();


		// set postprocess shader
		auto program = GET_RESOURCE(Program, "shaders/postprocess.prog");
		if (program)
		{
			program->Use();
			program->SetUniform("blend", m_blend);
			program->SetUniform("params", m_params);
			
			//ASSIGNMENT
			program->SetUniform("rTint", m_rTint);
			program->SetUniform("bTint", m_bTint);
			program->SetUniform("gTint", m_gTint);
		}

		ENGINE.GetSystem<Gui>()->EndFrame();
	}

	void World06::Draw(Renderer& renderer)
	{
		// **** PASS 1 *****
		m_scene->GetActorByName("postprocess")->active = false;

		auto framebuffer = GET_RESOURCE(Framebuffer, "fb");
		renderer.SetViewport(framebuffer->GetSize().x, framebuffer->GetSize().y);
		framebuffer->Bind();

		renderer.BeginFrame(glm::vec3{ 0.5, 0.8, 0.5 });
		m_scene->Draw(renderer);

		framebuffer->Unbind();

		// ***** PASS 2 *****
		m_scene->GetActorByName("postprocess")->active = true;

		renderer.ResetViewport();
		renderer.BeginFrame();
		m_scene->GetActorByName("postprocess")->Draw(renderer);

		// post-render
		ENGINE.GetSystem<Gui>()->Draw();
		renderer.EndFrame();
	}
}
