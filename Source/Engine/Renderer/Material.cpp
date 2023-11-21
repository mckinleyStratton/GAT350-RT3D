#include "Material.h"
#include "Program.h"
#include "Texture.h"
#include "Cubemap.h"
#include "Core/Core.h"
#include <imgui/ImFileDialog.h>

namespace nc
{
	bool Material::Create(std::string filename, ...)
	{
		// load program json document
		rapidjson::Document document;
		bool success = Json::Load(filename, document);
		if (!success)
		{
			INFO_LOG("Could not load program file (%s)." << filename);
			return false;
		}

		// read the program name
		std::string program;
		READ_DATA(document, program);
		// get program resource
		m_program = GET_RESOURCE(Program, program);

		// read the textures name
		std::string albedoTextureName;
		READ_NAME_DATA(document, "albedoTexture", albedoTextureName);
		if (!albedoTextureName.empty())
		{
			albedoTexture = GET_RESOURCE(Texture, albedoTextureName);
		}

		std::string specularTextureName;
		READ_NAME_DATA(document, "specularTexture", specularTextureName);
		if (!specularTextureName.empty())
		{
			specularTexture = GET_RESOURCE(Texture, specularTextureName);
		}

		std::string emissiveTextureName;
		READ_NAME_DATA(document, "emissiveTexture", emissiveTextureName);
		if (!emissiveTextureName.empty())
		{

			emissiveTexture = GET_RESOURCE(Texture, emissiveTextureName);
		}

		std::string normalTextureName;
		READ_NAME_DATA(document, "normalTexture", normalTextureName);
		if (!normalTextureName.empty())
		{

			normalTexture = GET_RESOURCE(Texture, normalTextureName);
		}


		std::string cubemapName;
		if (READ_NAME_DATA(document, "cubemap", cubemapName))
		{
			std::vector<std::string> cubemaps;
			READ_DATA(document, cubemaps);
			cubemapTexture = GET_RESOURCE(Cubemap, cubemapName, cubemaps);
		}

		READ_DATA(document, albedo);
		READ_DATA(document, specular);
		READ_DATA(document, shininess);
		READ_DATA(document, emissive);
		READ_DATA(document, tiling);
		READ_DATA(document, offset);

		return true;
	}

	void Material::Bind()
	{
		m_program->Use();
		m_program->SetUniform("material.params", params);
		m_program->SetUniform("material.albedo", albedo);
		m_program->SetUniform("material.specular", specular);
		m_program->SetUniform("material.shininess", shininess);
		m_program->SetUniform("material.emissive", emissive);
		m_program->SetUniform("material.tiling", tiling);
		m_program->SetUniform("material.offset", offset);

		if (albedoTexture)
		{
			params |= ALBEDO_TEXTURE_MASK;
			albedoTexture->SetActive(GL_TEXTURE0);
			albedoTexture->Bind();
		}

		if (specularTexture)
		{
			params |= SPECULAR_TEXTURE_MASK;

			specularTexture->SetActive(GL_TEXTURE1);
			specularTexture->Bind();
		}

		if (normalTexture)
		{
			params |= NORMAL_TEXTURE_MASK;

			normalTexture->SetActive(GL_TEXTURE2);
			normalTexture->Bind();
		}

		if (emissiveTexture)
		{
			params |= EMISSIVE_TEXTURE_MASK;

			emissiveTexture->SetActive(GL_TEXTURE3);
			emissiveTexture->Bind();
		}		
		if (cubemapTexture)
		{
			params |= CUBEMAP_TEXTURE_MASK;

			cubemapTexture->SetActive(GL_TEXTURE4);
			cubemapTexture->Bind();
		}
		if (depthTexture)
		{
			depthTexture->SetActive(GL_TEXTURE5);
			depthTexture->Bind();
		}

	}
	void Material::ProcessGui()
	{
		// shader
		ImGui::Text("Shader:");
		ImGui::SameLine();
		ImGui::Text("%s", m_program->name.c_str());
		Gui::GetDialogResource<Program>(m_program, "ShaderTextureDialog", "Open Shader", "Shader file (*.prog){.prog},.*");

		// albedo
		ImGui::TextColored(ImVec4{ 0, 1, 0, 1 }, "Name: %s", name.c_str());
		ImGui::Text("Albedo  ");
		ImGui::SameLine();
		ImGui::ColorEdit3("Albedo", glm::value_ptr(albedo), ImGuiColorEditFlags_NoLabel | ImGuiColorEditFlags_NoInputs);
		ImGui::SameLine();
		(albedoTexture) ? ImGui::Text("%s", albedoTexture->name.c_str()) : ImGui::Text("None");
		Gui::GetDialogResource<Texture>(albedoTexture, "AlbedoTextureDialog", "Open texture", "Image file (*.png;*.jpg;*.jpeg;*.bmp;*.tga){.png,.jpg,.jpeg,.bmp,.tga},.*");



		// specualr
		ImGui::TextColored(ImVec4{ 0, 1, 0, 1 }, "Name: %s", name.c_str());
		ImGui::Text("Specular  ");
		ImGui::SameLine();
		ImGui::ColorEdit3("Specualr", glm::value_ptr(specular), ImGuiColorEditFlags_NoLabel | ImGuiColorEditFlags_NoInputs);
		ImGui::SameLine();
		(specularTexture) ? ImGui::Text("%s", specularTexture->name.c_str()) : ImGui::Text("None");
		ImGui::DragFloat("Shininess", &shininess, 0.1f, 2.0f, 200.0f);
		Gui::GetDialogResource<Texture>(specularTexture, "SpecularTextureDialog", "Open texture", "Image file (*.png;*.jpg;*.jpeg;*.bmp;*.tga){.png,.jpg,.jpeg,.bmp,.tga},.*");

		// emissive
		ImGui::TextColored(ImVec4{ 0, 1, 0, 1 }, "Name: %s", name.c_str());
		ImGui::Text("Emissive  ");
		ImGui::SameLine();
		ImGui::ColorEdit3("Emissive", glm::value_ptr(emissive), ImGuiColorEditFlags_NoLabel | ImGuiColorEditFlags_NoInputs);
		ImGui::SameLine();
		(emissiveTexture) ? ImGui::Text("%s", emissiveTexture->name.c_str()) : ImGui::Text("None");
		Gui::GetDialogResource<Texture>(emissiveTexture, "EmissiveTextureDialog", "Open texture", "Image file (*.png;*.jpg;*.jpeg;*.bmp;*.tga){.png,.jpg,.jpeg,.bmp,.tga},.*");


		//  normal
		ImGui::Text("Normal  ");
		(normalTexture) ? ImGui::Text("%s", normalTexture->name.c_str()) : ImGui::Text("None");
		Gui::GetDialogResource<Texture>(normalTexture, "NormalTextureDialog", "Open texture", "Image file (*.png;*.jpg;*.jpeg;*.bmp;*.tga){.png,.jpg,.jpeg,.bmp,.tga},.*");


		// uv
		ImGui::DragFloat2("Tiling", glm::value_ptr(tiling), 0.1f);
		ImGui::DragFloat2("Offset", glm::value_ptr(offset), 0.1f);
	}
}