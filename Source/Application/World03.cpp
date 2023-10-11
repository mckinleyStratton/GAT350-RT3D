#include "World03.h"
#include "Framework/Framework.h"
#include "Input/InputSystem.h"

#include <glm/glm/gtc/type_ptr.hpp>

#define INTERLEAVE

namespace nc
{
    bool World03::Initialize()
    {

        m_program = GET_RESOURCE(Program, "shaders/unlit_texture.prog");
        m_program->Use();
        
        m_texture = GET_RESOURCE(Texture, "textures/llama.jpg");
        m_texture->Bind();
        m_texture->SetActive(GL_TEXTURE0);


#ifdef INTERLEAVE

       //vertex data
       // first three are position - then color, the uvs
        float vertexData[] = {
         -0.8f, -0.8f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
         -0.8f,  0.8f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
          0.8f, -0.8f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
          0.8f,  0.8f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f

        };

        // create buffer
        GLuint vbo; //Vertex Buffer Object -> buffer is just a block of memory
        glGenBuffers(1, &vbo); // get address of vbo
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);

        glGenVertexArrays(1, &m_vao);
        glBindVertexArray(m_vao);

        glBindVertexBuffer(0, vbo, 0, 8 * sizeof(GLfloat));
        
        // position
        glEnableVertexAttribArray(0);
        glVertexAttribFormat(0, 3, GL_FLOAT, GL_FALSE, 0);
        glVertexAttribBinding(0, 0);

        // color
        glEnableVertexAttribArray(1);
        glVertexAttribFormat(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat));
        glVertexAttribBinding(1, 0);

        // texcoord
        glEnableVertexAttribArray(2);
        glVertexAttribFormat(2, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat));
        glVertexAttribBinding(2, 0);


#else
        //vertex data
        float positionData[] = {
            -0.8f,  -0.8f, 0.0f,
            -0.8f,   0.8f, 0.0f,
             0.8f,  -0.8f, 0.0f,
             0.8f,   0.8f, 0.0f
        };

        //color data
        float colorData[] =
        {
            1.0f, 0.0f, 0.0f,
            0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 1.0f,
            1.0f, 1.0f, 1.0f
        };

        // create buffer
        GLuint vbo[2]; //Vertex Buffer Object -> buffer is just a block of memory
        glGenBuffers(2, vbo); // get address of vbo
        glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(positionData), positionData, GL_STATIC_DRAW);

        glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(colorData), colorData, GL_STATIC_DRAW);

        glGenVertexArrays(1, &m_vao);
        glBindVertexArray(m_vao);
        
        // position
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
        glBindVertexBuffer(0, vbo[0], 0, 3 * sizeof(GLfloat));

        // color
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
        glBindVertexBuffer(1, vbo[1], 0, 3 * sizeof(GLfloat));
#endif
        //m_position.z = -10.0f;
 
        return true;
    }

    void World03::Shutdown()
    {
    }

    void World03::Update(float dt)
    {
        ENGINE.GetSystem<Gui>()->BeginFrame();

        ImGui::Begin("Transform");
        ImGui::DragFloat3("Position", &m_transform.position[0]);
        ImGui::DragFloat3("Rotation", &m_transform.rotation[0]);
        ImGui::DragFloat3("Scale", &m_transform.scale[0]);
        ImGui::End();
        
        //m_transform.rotation.z += 180 * dt;

        m_transform.position.x += ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_A) ? m_speed * -dt : 0;
        m_transform.position.x += ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_D) ? m_speed * +dt : 0;
        m_transform.position.z += ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_W) ? m_speed * -dt : 0;
        m_transform.position.z += ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_S) ? m_speed * +dt : 0;

        m_time += dt;
        
        //model matrix
        m_program->SetUniform("model", m_transform.GetMatrix());


        //view matrix
        glm::mat4 view = glm::lookAt(glm::vec3{ 0, 4, 5}, glm::vec3{ 0, 0, 0 }, glm::vec3{ 0, 1, 0 });
        m_program->SetUniform("view", view);


        // projection matrix
        glm::mat4 projection = glm::perspective(glm::radians(70.0f), 800.0f / 600.0f, 0.0f, 100.0f);
        m_program->SetUniform("projection", projection);

        ENGINE.GetSystem<Gui>()->EndFrame();
    }

    void World03::Draw(Renderer& renderer)
    {
        // pre-render
        renderer.BeginFrame();
        
        // render
        glBindVertexArray(m_vao);
        //glDrawArrays(GL_TRIANGLES, 0, 3);
        //glDrawArrays(GL_QUADS, 0, 4);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

        ENGINE.GetSystem<Gui>()->Draw();

        // post-render
        renderer.EndFrame();
    }
}
