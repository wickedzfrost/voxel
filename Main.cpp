#include <array>
#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Core/Render/VAO.h"
#include "Core/Render/VBO.h"
#include "Core/Render/EBO.h"
#include "Core/Render/Shader.h"
#include "Core/Render/Texture.h"

// Callback function forward declarations
void processInput(GLFWwindow* window);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

namespace Configs
{
    inline constexpr int SCR_WIDTH{ 1200 };
    inline constexpr int SCR_HEIGHT{ 900 };
}

constexpr std::array<GLfloat, 32> vertices
{
    // Positions        // Colors           // Texture coords
     0.5f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f,   1.0f, 1.0f,
     0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   1.0f, 0.0f,
    -0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   0.0f, 0.0f,
    -0.5f,  0.5f, 0.0f,  1.0f, 1.0f, 0.0f,   0.0f, 1.0f,
};

constexpr std::array<GLuint, 6> indices
{
    0, 1, 3,
    1, 2, 3,
};

GLFWwindow* setupGLFW()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window{ glfwCreateWindow(Configs::SCR_WIDTH, Configs::SCR_HEIGHT, "Voxel", nullptr, nullptr) };
    if (window)
        return window;

    return nullptr;
}

// You need to manually sets the function pointers!
void setCallback(GLFWwindow* window)
{
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
}

bool loadGLAD()
{
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        return false;

    return true;
}

int main()
{
    GLFWwindow* window{ setupGLFW() };

    // Checks for nullptr
    if (!window)
    {
        std::cout << "Failed to create GLFW window\n";
        glfwTerminate();
        return -1;
    }

    // Sets the OpenGL context to the window
    glfwMakeContextCurrent(window);

    // This function handles all the callback setup
    setCallback(window);

    // Loads GLAD function pointers
    if (!loadGLAD())
    {
        std::cout << "Failed to load GLAD\n";
        glfwTerminate();
        return -1;
    }

    // Viewport setup
    glViewport(0, 0, Configs::SCR_WIDTH, Configs::SCR_HEIGHT);

    Shader shader("Shaders/vert.glsl", "Shaders/frag.glsl");

    VAO vao{};
    vao.Bind();

    VBO vbo{ vertices, GL_STATIC_DRAW };
    EBO ebo{ indices, GL_STATIC_DRAW };
    vbo.Bind();
    ebo.Bind();

    Texture texture{ "Assets/container.jpg", GL_RGB, GL_TEXTURE0 };
    Texture texture2{ "Assets/awesomeface.png", GL_RGBA, GL_TEXTURE1 };

    vao.LinkAttrib(vbo, 0, 3, GL_FLOAT, 8 * sizeof(GLfloat), 0);
    vao.LinkAttrib(vbo, 1, 3, GL_FLOAT, 8 * sizeof(GLfloat), 3 * sizeof(float));
    vao.LinkAttrib(vbo, 2, 3, GL_FLOAT, 8 * sizeof(GLfloat), 6 * sizeof(float));

    vao.Unbind();
    vbo.Unbind();
    ebo.Unbind();

    texture.TexUnit(shader, "texture1", 0);
    texture2.TexUnit(shader, "texture2", 1);

// Turns wireframe mode on or off
#if 0
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
#endif

    // Render loop
    while (!glfwWindowShouldClose(window))
    {
        processInput(window);

        // Draw calls
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        shader.Use();
        texture.Bind();
        texture2.Bind();
        vao.Bind();
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);;

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}