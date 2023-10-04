#include "World01.h"
#include "Framework/Framework.h"
#include "Renderer/Renderer.h"
#include "Input/InputSystem.h"

namespace nc
{
    bool World01::Initialize()
    {
        for (int i = 0; i < 10; i++)
        {
            m_positions.push_back({ randomf(-1, 1), randomf(-1, 1) });
        }


        return true;
    }

    void World01::Shutdown()
    {
    }

    void World01::Update(float dt)
    {
        m_angle += 90 * dt;

        m_position.x += ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_A) ? -dt : 0;
        m_position.x += ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_D) ? +dt : 0;
        m_time += dt;
    }

    void World01::Draw(Renderer& renderer)
    {
        // pre-render
        renderer.BeginFrame();

       for (int i = 0; i < 10; i++)
        {
            // render
            glPushMatrix();
            glTranslatef(m_positions[i].x, m_positions[i].y, 0);
            
            glRotatef(m_angle, 0, 1, 0);
            glScalef( 0.05f, 0.05f, 1);

            // OpenGL Hello World!
            glBegin(GL_POLYGON);
            glColor3f(1, 0.6f, 1);
            glVertex2f(0, 1);
            glColor3f(0, 1, 0);
            glVertex2f(-0.2, 0.6);
            glVertex2f(-1, 0.6);
            glVertex2f(-0.4, 0.2);
            glColor3f(1, 0, 0);
            glVertex2f(-0.6, -0.6);
            glColor3f(1, 0, 1);
            glVertex2f(0, -0.2);
            glVertex2f(0.6, -0.6);
            glColor3f(0, 1, 1);
            glVertex2f(0.4, 0.2);
            glColor3f(1, 1, 0);
            glVertex2f(1, 0.6);
            glVertex2f(0.2, 0.6);


            /* For Triangles 
            * 
            glRotatef(m_angle, 1, 1, 1); // for triangle
            glScalef((sin(m_time * 0.5f) + 0.05f) * 0.1f, 0.1f, 1); // for triangle

            glBegin(GL_TRIANGLES);

            //3f RGB betweet 0 and 1
            glColor3f(1, 0, 0);
            //2f means 2 floats - 2d is doubles
            glVertex2f(-0.5f, -0.1f);

            glColor3f(0, 1, 0);
            glVertex2f(0.5f, 0.5f);

            glColor3f(0, 0, 1);
            glVertex2f(-0.2f, 0.8f);

            glColor3f(1, 1, 1);
            glVertex2f(-0.3f, 0.9f);

            glColor3f(1, 1, 0);
            glVertex2f(-0.5f, 0.5f);

            glColor3f(0, 1, 0);
            glVertex2f(-0.9f, 0.9f);
            */

            glEnd();
            glPopMatrix();
        }


        // post-render
        renderer.EndFrame();
    }
}
