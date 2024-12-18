#define GLM_ENABLE_EXPERIMENTAL

#include "GLWindow.h"
#include "Camera.h"
#include "FrameBuffer.h"
#include "Renderer.h"

#include <ctime>

int main(int, char**)
{
    //setup main window with glfw
    GLWindow gl_window;

    Camera camera = Camera(glm::vec3(5, 0, 0), glm::vec3(0, 0, 0), &gl_window.WIDTH, &gl_window.HEIGHT);
    //generate buffers (frameB, textureB, renderB) and bind them
    FrameBuffer frameBuffer(gl_window.WIDTH, gl_window.HEIGHT);

    gl_window.InitCamera(&camera, &frameBuffer);

    Renderer renderer = Renderer(&frameBuffer, &camera);

    long int t = std::time(NULL);
    while (!glfwWindowShouldClose(gl_window.window))
    {
        if (std::time(NULL) - t >= 1) {
            t = std::time(NULL);

            renderer.testUpdate();
        }
        renderer.renderScene();


        //swap buffers and process each pending event
        glfwSwapBuffers(gl_window.window);
        glfwPollEvents();
    }
    return 0;
}
