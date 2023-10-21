#include "World04.h"
#include "Framework/Framework.h"
#include "Input/InputSystem.h"
#include "Renderer/VertexBuffer.h"


#include <glm/glm/gtc/type_ptr.hpp>

namespace nc
{
    bool World04::Initialize()
    {
        auto material = GET_RESOURCE(Material, "materials/grid.mtrl");
        m_model = std::make_shared<Model>();
        m_model->SetMaterial(material);
        m_model->Load("models/buddha.obj", glm::vec3{ 0 }, glm::vec3{-90, 0, 0});

        return true;
    }

    void World04::Shutdown()
    {
    }

    void World04::Update(float dt)
    {
        ENGINE.GetSystem<Gui>()->BeginFrame();

        ImGui::Begin("Transform");
        ImGui::DragFloat3("Position", &m_transform.position[0], 1.0f);
        ImGui::DragFloat3("Rotation", &m_transform.rotation[0]);
        ImGui::DragFloat3("Scale", &m_transform.scale[0], 1.0f);
        ImGui::End();

        ImGui::Begin("Light");
        ImGui::DragFloat3("Position", glm::value_ptr(m_lightPosition), 1.0f);
        ImGui::DragFloat3("Color", glm::value_ptr(m_lightColor));
        ImGui::DragFloat3("Ambient Color", glm::value_ptr(m_ambientColor));
        ImGui::End();
        
        //m_transform.rotation.z += 180 * dt;

        m_transform.position.x += ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_A) ? m_speed * -dt : 0;
        m_transform.position.x += ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_D) ? m_speed * +dt : 0;
        m_transform.position.z += ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_W) ? m_speed * -dt : 0;
        m_transform.position.z += ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_S) ? m_speed * +dt : 0;

        m_time += dt;

        auto material = m_model->GetMaterial();

        material->ProcessGui();
        material->Bind();

        material->GetProgram()->SetUniform("light.position", m_lightPosition);
        material->GetProgram()->SetUniform("light.color", m_lightColor);
        material->GetProgram()->SetUniform("ambientLight", m_ambientColor);

        //model matrix
        material->GetProgram()->SetUniform("model", m_transform.GetMatrix());

        //view matrix
        // function below is the camera view
        glm::mat4 view = glm::lookAt(glm::vec3{ 0, 0, 5}, glm::vec3{ 0, 0, 0 }, glm::vec3{ 0, 1, 0 });
        material->GetProgram()->SetUniform("view", view);


        // projection matrix
        glm::mat4 projection = glm::perspective(glm::radians(70.0f), ENGINE.GetSystem<Renderer>()->GetWidth() / (float)ENGINE.GetSystem<Renderer>()->GetHeight(), 0.01f, 100.0f);
        material->GetProgram()->SetUniform("projection", projection);



        // get material get light GUI - HOMEWORK
        
        
        
        
        //ImGui::Begin("Light Controls");

        ////ambient
        //glm::vec3 ambientLight = { 1.0f, 1.0f, 1.0f };
        //ImGui::DragFloat3("Ambient Light", glm::value_ptr(ambientLight));
        //material->GetProgram()->SetUniform("ambientLight", ambientLight);

        //// position
        //glm::vec3 lightPosition = { 1.0f, 1.0f, 1.0f };
        //ImGui::DragFloat3("Light Position", glm::value_ptr(lightPosition));
        //material->GetProgram()->SetUniform("light.position", lightPosition);

        ////diffuse
        //glm::vec3 lightColor = { 1.0f, 1.0f, 1.0f };
        //ImGui::ColorEdit3("Light Diffuse Color", glm::value_ptr(lightColor));
        //material->GetProgram()->SetUniform("light.color", lightColor);


        //ImGui::End();


        ENGINE.GetSystem<Gui>()->EndFrame();
        
    }

    void World04::Draw(Renderer& renderer)
    {
        // pre-render
        renderer.BeginFrame();

        // render
        //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        m_model->Draw(GL_TRIANGLES);
        ENGINE.GetSystem<Gui>()->Draw();

        // post-render
        renderer.EndFrame();
    }
}
