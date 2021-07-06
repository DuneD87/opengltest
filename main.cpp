
#define STB_IMAGE_IMPLEMENTATION
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "GameClasses/XmlParser.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "GameClasses/Model.h"
#include "GameClasses/Shader.h"
#include "GameClasses/Camera.h"
#include "GameClasses/BasicShapeBuilder.h"
#include <iostream>


// settings
const unsigned int SCR_WIDTH = 1024;
const unsigned int SCR_HEIGHT = 720;
bool spotLightEnabled = false;
float lastX = SCR_WIDTH/2, lastY = SCR_HEIGHT/2,pitch = 0, yaw = -90, fov = 45;

Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));

float deltaTime = 0.0f;	// Time between current frame and last frame
float lastFrame = 0.0f; // Time of last frame

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
    if (glfwGetKey(window,GLFW_KEY_SPACE) == GLFW_PRESS)
        camera.ProcessKeyboard(UP,deltaTime);
    if (glfwGetKey(window,GLFW_KEY_C) == GLFW_PRESS)
        camera.ProcessKeyboard(DOWN,deltaTime);
    if (glfwGetKey(window,GLFW_KEY_L) == GLFW_PRESS)
        spotLightEnabled = !spotLightEnabled;
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);

}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(yoffset);
}

void renderLoopCamera(Shader shader)
{
    //view
    glm::mat4 view;
    view = camera.GetViewMatrix();
    //projection

    shader.setMat4("view",view);
}

// ---------------------------------------------------
int main()
{
    XmlParser parser("../Scenes/Scene1.xml");
    vector<Light> lights = parser.getLights();
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }


    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    stbi_set_flip_vertically_on_load(true);
    glEnable(GL_DEPTH_TEST);
    // build and compile our shader zprogram
    // ------------------------------------
    Shader lightShader("../Shaders/lightingShader.vs", "../Shaders/lightingShader.fs");
    Shader cubeLightShader("../Shaders/lightingShader.vs", "../Shaders/noLightObjects/lightingShader.fs");
    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------

    Cube cube;
    Mesh cubeMesh = procesMesh(cube.vertices, cube.indices,"container2.png","../Models",208,72);
    Model capsule;
    capsule.meshes.push_back(cubeMesh);
    Cube cube2;
    Mesh cubeMesh2 = procesMesh(cube.vertices, cube.indices,"container2.png","../Models",208,72);
    Model capsule2;
    capsule2.meshes.push_back(cubeMesh2);
    capsule2.setPosition(glm::vec3(3,0,0));
    capsule.meshes.push_back(cubeMesh);
    lightShader.use();
    glm::mat4 projection;
    projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH/SCR_HEIGHT, 0.1f, 100.0f);
    lightShader.setMat4("projection",projection);
    //Render loop
    //Model backpack("../Models/cube.obj");
    Light spotLight(SpotLight,glm::vec3(0.0f, 0.0f, 0.0f),glm::vec3(1.0f, 1.0f, 1.0f),
                    glm::vec3(1.0f, 1.0f, 1.0f),1.0,0.09,0.032);
    while (!glfwWindowShouldClose(window))
    {
        // input
        // -----
        processInput(window);

        // render
        // ------

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        lightShader.use();
        lightShader.setFloat("material.shininess", 64.0f);
        lightShader.setVec3("viewPos",camera.Position);
        lightShader.addLights(lights);
        // spotLight
        if (spotLightEnabled)
            lightShader.addSpotLight(spotLight,camera.Front,camera.Position,glm::cos(glm::radians(12.5f))
                                 ,glm::cos(glm::radians(15.0f)));
        else
            lightShader.disableSpotLight();

        renderLoopCamera(lightShader);
        capsule.setPosition(glm::vec3(0,0,glfwGetTime()*10));
        capsule.Draw(lightShader);
        capsule2.Draw(lightShader);
        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}
