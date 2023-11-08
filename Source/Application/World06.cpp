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

		{
			auto actor = CREATE_CLASS(Actor);
			actor->name = "light1";
			actor->transform.position = glm::vec3{ 3, 3, 3 };


			auto lightComponent = CREATE_CLASS(LightComponent);
			lightComponent->type = LightComponent::eType::Point;
			lightComponent->color = glm::vec3{ 1, 1, 1 }; // glm::rgbColor(glm::vec3{ randomf() * 360, 1, 1 });
			lightComponent->intensity = 1;
			lightComponent->range = 20;
			lightComponent->innerAngle = 10.0f;
			lightComponent->outerAngle = 30.0f;
			actor->AddComponent(std::move(lightComponent));

			m_scene->Add(std::move(actor));
		}

		{
			auto actor = CREATE_CLASS(Actor);
			actor->name = "camera1";
			actor->transform.position = glm::vec3{ 0, 0, 18 };
			actor->transform.rotation = glm::radians(glm::vec3{ 0, 180, 0 });

			auto cameraComponent = CREATE_CLASS(CameraComponent);
			cameraComponent->SetPerspective(70.0f, ENGINE.GetSystem<Renderer>()->GetWidth() / (float)ENGINE.GetSystem<Renderer>()->GetHeight(), 0.1f, 100.0f);
			actor->AddComponent(std::move(cameraComponent));

			auto cameraController = CREATE_CLASS(CameraController);
			cameraController->speed = 5;
			cameraController->sensitivity = 0.5f;
			cameraController->m_owner = actor.get();
			cameraController->Initialize();
			actor->AddComponent(std::move(cameraController));


			m_scene->Add(std::move(actor));
		}

		for (int i = 0; i < 5; i++)
		{

			auto actor = CREATE_CLASS_BASE(Actor, "tree");
			actor->name = nc::CreateUnique("tree");
			actor->transform.scale = glm::vec3{ randomf(0.5f, 3.0f), randomf(0.5f, 3.0f), 0 };


			actor->transform.position = glm::vec3{ randomf(-5, 5), 0, randomf(-5, 5) };
			actor->Initialize();
			m_scene->Add(std::move(actor));

		}


		return true;
	};


	void World06::Shutdown()
	{
	}

	void World06::Update(float dt)
	{
		ENGINE.GetSystem<Gui>()->BeginFrame();

		m_scene->Update(dt);
		m_scene->ProcessGui();

		//m_transform.rotation.z += 0 * dt;

		auto actor = m_scene->GetActorByName<Actor>("actor1");
		// extra actors
		actor = m_scene->GetActorByName<Actor>("actor2");
		actor = m_scene->GetActorByName<Actor>("actor3");

		//actor->transform.position.x += ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_A) ? m_speed * -dt : 0;
		//actor->transform.position.x += ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_D) ? m_speed * +dt : 0;
		//actor->transform.position.z += ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_W) ? m_speed * -dt : 0; // going into screen 
		//actor->transform.position.z += ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_S) ? m_speed * +dt : 0; // coming out of screen

		
		auto material = actor->GetComponent<ModelComponent>()->material;

		auto program = material->GetProgram();

		if (!material) {
			std::cerr << "Error: Material is null." << std::endl;
			return;
		}

		material->ProcessGui();
		material->Bind();

		material = GET_RESOURCE(Material, "materials/refraction.mtrl"); // shaders
		if (material)
		{
			ImGui::Begin("Refraction");

			m_refraction = 1.0f + std::fabs(std::sin(m_time));

			ImGui::DragFloat("IOR", &m_refraction, 0.01f, 1, 3);
			auto program = material->GetProgram();
			program->Use();
			program->SetUniform("ior", m_refraction);

			ImGui::End();

		}


		m_time += dt;

		ENGINE.GetSystem<Gui>()->EndFrame();

		// comment out for now
		// material->GetProgram()->SetUniform("ambientLight", m_ambientColor);
	}

	void World06::Draw(Renderer& renderer)
	{
		// pre-render
		renderer.BeginFrame();

		// render
		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		m_scene->Draw(renderer);

		ENGINE.GetSystem<Gui>()->Draw();

		// post-render
		renderer.EndFrame();
	}
}