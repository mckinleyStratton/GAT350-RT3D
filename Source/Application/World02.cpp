#include "World02.h"
#include "Framework/Framework.h"
#include "Input/InputSystem.h"

#define INTERLEAVE

namespace nc
{
    bool World02::Initialize()
    {
        // shaders
        const char* vertexShader =
            "#version 430\n"
            "layout (location=0) in vec3 position;"
            "layout (location=1) in vec3 color;"
            "layout (location=0) out vec3 ocolor;"
            "void main() {"
            "  ocolor = color;"
            "  gl_Position = vec4(position, 1.0);"
            "}";

        const char* fragmentShader =
            "#version 430\n"
            "layout (location=0) in vec3 color;"
            "out vec4 ocolor;"
            "void main() {"
            "  ocolor = vec4(color, 1);"
            "}";

        GLuint vs = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vs, 1, &vertexShader, NULL);
        glCompileShader(vs);

        GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fs, 1, &fragmentShader, NULL);
        glCompileShader(fs);

        // attach the shader
        GLuint program = glCreateProgram();
        glAttachShader(program, vs);
        glAttachShader(program, fs);
        // link them together
        glLinkProgram(program);
        // now tell it to use the program
        glUseProgram(program);

#ifdef INTERLEAVE

        //vertex data
        float vertexData[] = {
             -0.8f, -0.8f, 0.0f, 1.0f, 0.0f, 0.0f,
             0.8f, -0.8f, 0.0f, 0.0f, 1.0f, 0.0f,
             0.8f,  0.8f, 0.0f, 0.0f, 0.0f, 1.0f,
             -0.8f,  0.8f, 0.0f, 1.0f, 1.0f, 1.0f
        };

        // create buffer
        GLuint vbo; //Vertex Buffer Object -> buffer is just a block of memory
        glGenBuffers(1, &vbo); // get address of vbo
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);

        glGenVertexArrays(1, &m_vao);
        glBindVertexArray(m_vao);

        glBindVertexBuffer(0, vbo, 0, 6 * sizeof(GLfloat));
        
        // position
        glEnableVertexAttribArray(0);
        glVertexAttribFormat(0, 3, GL_FLOAT, GL_FALSE, 0);
        glVertexAttribBinding(0, 0);

        // color
        glEnableVertexAttribArray(1);
        glVertexAttribFormat(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat));
        glVertexAttribBinding(1, 0);

#else
        //vertex data
        float positionData[] = {
            -0.8f, -0.8f, 0.0f,
             0.8f, -0.8f, 0.0f,
             0.8f,  0.8f, 0.0f,
             -0.8f,  0.8f, 0.0f
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

 
        return true;
    }

    void World02::Shutdown()
    {
    }

    void World02::Update(float dt)
    {
        m_angle += 90 * dt;

        m_position.x += ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_A) ? -dt : 0;
        m_position.x += ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_D) ? +dt : 0;
        m_time += dt;
    }

    void World02::Draw(Renderer& renderer)
    {
        // pre-render
        renderer.BeginFrame();
        
        // render
        glBindVertexArray(m_vao);
        //glDrawArrays(GL_TRIANGLES, 0, 3);
        glDrawArrays(GL_QUADS, 0, 4);
        //glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

        // post-render
        renderer.EndFrame();
    }
}
