#pragma once
#include "Framework/World.h"
#include "Renderer/Renderer.h"
#include "Core/Math/Transform.h"

#include <vector>

namespace nc
{
	class World04 : public World
	{
	public:
		bool Initialize() override;
		void Shutdown() override;
		void Update(float dt) override;
		void Draw(Renderer& renderer) override;

	private:
		float m_time = 0;
		float m_speed = 10;

		glm::vec3 m_lightPosition{ 0, 8, 0 };
		glm::vec3 m_lightColor{ 1 };
		glm::vec3 m_ambientColor{ 0.2f };
		glm::vec3 ambientLight{ 1 };

		Transform m_transform;

		GLuint m_vao; //Vertex Array Object
		res_t<Program> m_program;
		res_t<Texture> m_texture;
		res_t<Material> m_material;
		res_t<VertexBuffer> m_vertexBuffer;
		res_t<Model> m_model;

	};
}