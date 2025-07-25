#include <array>
#include <cmath>
#include <iostream>
#include <numbers>

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
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xPos, double yPos);
void scroll_callback(GLFWwindow* window, [[maybe_unused]] double xOffset, double yOffset);
void processInput(GLFWwindow* window);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

namespace Configs
{
    inline constexpr int SCR_WIDTH{ 1200 };
    inline constexpr int SCR_HEIGHT{ 900 };
    inline constexpr float ASPECT_RATIO{ static_cast<float>(SCR_WIDTH) / SCR_HEIGHT };
}

namespace Globals
{
    Camera g_camera(glm::vec3(0.0f, 0.0f, 3.0f));
    float g_deltaTime{ 0.0f };
    float g_lastTime{ 0.0f };
    float g_animationTime{ 0.0f };
    bool g_animationPaused{ false };
    glm::vec3 g_lastLightPos{ 1.0f, 0.4f, 1.5f };

    bool g_firstMouse{ true };
    float g_mouseLastX{ Configs::SCR_WIDTH };
    float g_mouseLastY{ Configs::SCR_HEIGHT };

    bool g_enableWireframe{ false };
    bool g_enableLightMove{ true };
}

constexpr std::array<GLfloat, 288> vertices
{
    // Position           // Normal            // Tex Coord
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
};

// Anon-namespace for initialization functions
namespace
{
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
        glfwSetCursorPosCallback(window, mouse_callback);
        glfwSetScrollCallback(window, scroll_callback);
        glfwSetKeyCallback(window, key_callback);
    }

    bool loadGLAD()
    {
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
            return false;

        return true;
    }
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

    // Captures our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // Loads GLAD function pointers
    if (!loadGLAD())
    {
        std::cout << "Failed to load GLAD\n";
        glfwTerminate();
        return -1;
    }

    // Viewport setup
    glViewport(0, 0, Configs::SCR_WIDTH, Configs::SCR_HEIGHT);

    // Creates shader program
    Shader lightingShader("Shaders/lightingVert.glsl", "Shaders/lightingFrag.glsl");
    Shader lightCubeShader("Shaders/lightCubeVert.glsl", "Shaders/lightCubeFrag.glsl");

    // Initialize cube's VAO and VBO
    VAO cubeVao{};
    cubeVao.Bind();

    VBO cubeVbo{ vertices, GL_STATIC_DRAW };
    cubeVbo.Bind();
    cubeVao.LinkAttrib(cubeVbo, 0, 3, GL_FLOAT, 8 * sizeof(GLfloat), 0);                    // Position
    cubeVao.LinkAttrib(cubeVbo, 1, 3, GL_FLOAT, 8 * sizeof(GLfloat), 3 * sizeof(GLfloat));  // Normal
    cubeVao.LinkAttrib(cubeVbo, 2, 2, GL_FLOAT, 8 * sizeof(GLfloat), 6 * sizeof(GLfloat));  // Texture Coord
    
    // Initialize light source's VAO and VBO
    VAO lightVAO{};
    lightVAO.Bind();
    
    cubeVbo.Bind();
    lightVAO.LinkAttrib(cubeVbo, 0, 3, GL_FLOAT, 8 * sizeof(GLfloat), 0);   // Position

    // Textures
    Texture container{ "Assets/container2.png", GL_RGBA, 0 };
    Texture containerSpec{ "Assets/container2_Specular.png", GL_RGBA, 1 };

    lightingShader.Use();
    lightingShader.SetInt("material.diffuse", 0);
    lightingShader.SetInt("material.specular", 1);

    // Unbind to prevent accidental modifications
    cubeVao.Unbind();
    lightVAO.Unbind();
    cubeVbo.Unbind();

    glEnable(GL_DEPTH_TEST);

    // Render loop
    while (!glfwWindowShouldClose(window))
    {
        // Enable or disable wireframe mode
        if (Globals::g_enableWireframe)
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        else
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        
        // Per-frame time logic
        float currentTime{ static_cast<float>(glfwGetTime()) };
        Globals::g_deltaTime = currentTime - Globals::g_lastTime;
        Globals::g_lastTime = currentTime;

        // Tracks animation time and resets it once the time passes 2 * PI
        if (!Globals::g_animationPaused)
        {
            Globals::g_animationTime += Globals::g_deltaTime;

            if (Globals::g_animationTime > 2.0f * std::numbers::pi_v<float>)
            {
                Globals::g_animationTime -= 2.0f * std::numbers::pi_v<float>;
            }
        }

        // Input
        processInput(window);

        // Render
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Configure light properties
        glm::vec3 lightPos{ Globals::g_lastLightPos };
        const glm::vec3 objectColor{ 1.0f, 0.5f, 0.31f };
        glm::vec3 lightColor{ 1.0f, 1.0f, 1.0f };

        if (Globals::g_enableLightMove)
        {
            constexpr float radius{ 2.5f };
            constexpr float height{ 0.4f };
            lightPos = glm::vec3{ radius * std::sin(Globals::g_animationTime), height, radius * std::cos(Globals::g_animationTime) };
            Globals::g_lastLightPos = lightPos;
        }

        // Activate shader and set uniforms
        lightingShader.Use();
        lightingShader.SetVec3("light.position", lightPos);
        lightingShader.SetVec3("viewPos", Globals::g_camera.m_cameraPosition);

        // Set light intensity
        const glm::vec3 diffuseColor{ lightColor * glm::vec3{ 0.5f } };
        const glm::vec3 ambientColor{ diffuseColor * glm::vec3{ 0.2f } };
        lightingShader.SetVec3("light.ambient", ambientColor);
        lightingShader.SetVec3("light.diffuse", diffuseColor);
        lightingShader.SetVec3("light.specular", glm::vec3{ 1.0f });

        // Set material quality
        lightingShader.SetVec3("material.specular", glm::vec3{ 0.5f });
        lightingShader.SetFloat("material.shininess", 64.0f);

        // View/projection matrix transformations
        constexpr float nearPlane{ 0.1f };
        constexpr float farPlane{ 100.0f };
        const glm::mat4 proj{ glm::perspective(glm::radians(Globals::g_camera.m_zoom), Configs::ASPECT_RATIO, nearPlane, farPlane) };
        const glm::mat4 view{ Globals::g_camera.GetViewMatrix() };
        lightingShader.SetMat4("proj", proj);
        lightingShader.SetMat4("view", view);

        // World transformation
        glm::mat4 model{ glm::mat4{1.0f} };
        model = glm::translate(model, glm::vec3{ 0.0f, -0.75f, 0.0f });
        lightingShader.SetMat4("model", model);

        // Bind texture
        container.Bind();
        containerSpec.Bind();

        // Render the cube
        cubeVao.Bind();
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // Render the light source as well
        lightCubeShader.Use();
        lightCubeShader.SetVec3("lightColor", lightColor);
        lightCubeShader.SetMat4("proj", proj);
        lightCubeShader.SetMat4("view", view);

        model = glm::mat4{ 1.0f };
        model = glm::translate(model, lightPos);
        model = glm::scale(model, glm::vec3{ 0.2f });
        
        lightCubeShader.SetMat4("model", model);

        lightVAO.Bind();
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // GLFW: Swap buffer and poll IO events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xPos, double yPos)
{
    using namespace Globals;

    float xPosition{ static_cast<float>(xPos) };
    float yPosition{ static_cast<float>(yPos) };

    if (g_firstMouse)
    {
        g_mouseLastX = xPosition;
        g_mouseLastY = yPosition;
        g_firstMouse = false;
    }

    float xOffset{ xPosition - g_mouseLastX };
    float yOffset{ g_mouseLastY - yPosition };

    g_mouseLastX = xPosition;
    g_mouseLastY = yPosition;

    g_camera.ProcessMouseMovement(xOffset, yOffset);
}

void scroll_callback(GLFWwindow* window, [[maybe_unused]] double xOffset, double yOffset)
{
    using namespace Globals;
    g_camera.ProcessScrollWheel(static_cast<float>(yOffset));
}

void processInput(GLFWwindow* window)
{
    using namespace Globals;
    using enum Camera::Direction;

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);

    // Handles movement
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        g_camera.ProcessKeyboard(forward, g_deltaTime);

    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        g_camera.ProcessKeyboard(backward, g_deltaTime);

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        g_camera.ProcessKeyboard(left, g_deltaTime);

    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        g_camera.ProcessKeyboard(right, g_deltaTime);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_E && action == GLFW_PRESS)
        Globals::g_enableWireframe = !Globals::g_enableWireframe;
    if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
    {
        Globals::g_enableLightMove = !Globals::g_enableLightMove;
        Globals::g_animationPaused = !Globals::g_animationPaused;
    }
}