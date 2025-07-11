#include <array>
#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Core/Render/VAO.h"
#include "Core/Render/VBO.h"
#include "Core/Render/EBO.h"
#include "Core/Render/Shader.h"
#include "Core/Render/Texture.h"
#include "Core/Render/Camera.h"

// Callback function forward declarations
void processInput(GLFWwindow* window);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

namespace Configs
{
    inline constexpr int SCR_WIDTH{ 1200 };
    inline constexpr int SCR_HEIGHT{ 900 };
    inline constexpr float ASPECT_RATIO{ static_cast<float>(SCR_WIDTH) / SCR_HEIGHT };
}

constexpr std::array<GLfloat, 192> vertices
{
    // Position           // Color            // Texture
     0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 1.0f,   1.0f, 1.0f,        // 0
     0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f,   1.0f, 0.0f,        // 1
    -0.5f,  0.5f, -0.5f,  1.0f, 0.0f, 0.0f,   0.0f, 0.0f,        // 2
    -0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.0f,   0.0f, 1.0f,        // 3

     0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f,   0.0f, 1.0f,        // 4
     0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 0.0f,   0.0f, 0.0f,        // 5
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 0.0f,   1.0f, 0.0f,        // 6
    -0.5f,  0.5f, -0.5f,  1.0f, 0.0f, 0.0f,   1.0f, 1.0f,        // 7

    -0.5f,  0.5f, -0.5f,  1.0f, 0.0f, 0.0f,   0.0f, 1.0f,        // 8
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 0.0f,   0.0f, 0.0f,        // 9
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0.0f,   1.0f, 0.0f,        // 10
    -0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.0f,   1.0f, 1.0f,        // 11

    -0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.0f,   0.0f, 1.0f,        // 12
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0.0f,   0.0f, 0.0f,        // 13
     0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0.0f,   1.0f, 0.0f,        // 14
     0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 1.0f,   1.0f, 1.0f,        // 15

     0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 1.0f,   0.0f, 1.0f,        // 16
     0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0.0f,   0.0f, 0.0f,        // 17
     0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 0.0f,   1.0f, 0.0f,        // 18
     0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f,   1.0f, 1.0f,        // 19

     0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0.0f,   1.0f, 1.0f,        // 20
     0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 0.0f,   1.0f, 0.0f,        // 21
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 0.0f,   0.0f, 0.0f,        // 22
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0.0f,   0.0f, 1.0f,        // 23
};

constexpr std::array<GLuint, 36> indices
{
    // Top face
    0, 1, 2,
    2, 3, 0,

    // Front face
    4, 5, 6,
    6, 7, 4,

    // Left face
    8, 9, 10,
    10, 11, 8,

    // Back face
    12, 13, 14,
    14, 15, 12,

    // Right face
    16, 17, 18,
    18, 19, 16,

    // Bottom face
    20, 21, 22,
    22, 23, 20,
};

constexpr std::array<glm::vec3, 10> cubePositions
{
    glm::vec3(0.0f,  0.0f,  0.0f),
    glm::vec3(2.0f,  5.0f, -15.0f),
    glm::vec3(-1.5f, -2.2f, -2.5f),
    glm::vec3(-3.8f, -2.0f, -12.3f),
    glm::vec3(2.4f, -0.4f, -3.5f),
    glm::vec3(-1.7f,  3.0f, -7.5f),
    glm::vec3(1.3f, -2.0f, -2.5f),
    glm::vec3(1.5f,  2.0f, -2.5f),
    glm::vec3(1.5f,  0.2f, -1.5f),
    glm::vec3(-1.3f,  1.0f, -1.5f)
};

Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));

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

    // Set once, as projection matrix will probably not be changed
    glm::mat4 proj{ glm::perspective(glm::radians(55.0f), Configs::ASPECT_RATIO, 0.1f, 100.0f) };
    shader.setMat4("proj", proj);

// Turns wireframe mode on or off
#if 0
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
#endif

    glEnable(GL_DEPTH_TEST);
    
    glm::mat4 view{ camera.GetViewMatrix() };
    
    // Render loop
    while (!glfwWindowShouldClose(window))
    {
        processInput(window);

        // Draw calls
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        view = camera.GetViewMatrix();
        shader.setMat4("view", view);

        shader.Use();
        vao.Bind();

        for (std::size_t i{ 0 }; i < cubePositions.size(); ++i)
        {
            glm::mat4 model{ glm::mat4{1.0f} };
            model = glm::translate(model, cubePositions[static_cast<int>(i)]);
            float angle{ 20.0f * i };
            model = glm::rotate(model, glm::radians(angle), glm::vec3(0.5f, 1.0f, 0.0f));
            shader.setMat4("model", model);

            glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

void processInput(GLFWwindow* window)
{
    using enum Camera::Direction;
    float deltaTime{ 0.1f };

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(forward, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(backward, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(left, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(right, deltaTime);

}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}